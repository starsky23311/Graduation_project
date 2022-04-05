#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <librealsense2/rs.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include "realsense/rs_factory.hpp"
#include <fstream>
#include <vector>
#include <Eigen/Core>
#include <pcl/console/print.h>
#include <pcl/io/pcd_io.h>
#include <boost/filesystem.hpp>
#include <flann/flann.h>
#include <flann/io/hdf5.h>
#include <fstream>
#include <pcl/console/parse.h>
#include <pcl/console/print.h>
#include <iostream>
#include <boost/algorithm/string/replace.hpp> // for replace_last
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <visp3/visual_features/vpFeatureBuilder.h>
#include <visp3/vs/vpServo.h>
#include <visp3/robot/vpSimulatorCamera.h>
#include <visp3/gui/vpDisplayX.h>
#include <visp3/gui/vpProjectionDisplay.h>
#include <visp3/vs/vpServoDisplay.h>
#include <visp3/gui/vpPlot.h>
#include <visp3/core/vpMatrix.h>
#include <algorithm>
#include <string>
class VSReady
{
private:
    bool AlignOK;
    bool ColorOK;
    bool VSOK;
public:
    VSReady()
    {
        AlignOK = false;
        ColorOK = false;
        VSOK = false;
    }
    void setAlignOK(bool setflag)
    {
        this->AlignOK = setflag;
        if(this->ColorOK == true)
            this->VSOK = true;
    }
    void setColorOK(bool setflag)
    {
        this->ColorOK = setflag;
        if(this->AlignOK == true)
            this->VSOK = true;
    }
    bool getVSOK(void)
    {
        return this->VSOK;
    }
    void refreshOK(void)
    {
        AlignOK = false;
        ColorOK = false;
        VSOK = false;
    }
    ~VSReady(){}
};
VSReady visualready;
bool compPointXUp(cv::Point& p1,cv::Point& p2)
{
    return (p1.x < p2.x);
}
bool compPointYUp(cv::Point& p1,cv::Point& p2)
{
    return (p1.y < p2.y);
}
bool compPointYDown(cv::Point& p1,cv::Point& p2)
{
    return (p1.y > p2.y);
}
bool compSquareSize(std::vector<cv::Point> S1,std::vector<cv::Point> S2)
{
    return fabs(contourArea(cv::Mat(S1))) < fabs(contourArea(cv::Mat(S2)));
}

class ModelBasedVisualServo : public rclcpp::Node
{
public:
    enum ModeType
    {
        SIM_VISP,
        RUN
    };
    #define LAMDA_SIMVISP 0.5
    #define LAMDA_RUN     1.30
    ModelBasedVisualServo(int argc, char * argv[])
            : Node("ModelBasedVisualServoNode")
    {
        if(pcl::console::find_switch (argc, argv, "--sim-visp"))
            mode = SIM_VISP;
        else if(pcl::console::find_switch (argc, argv, "--run"))
            mode = RUN;


        VisualServoInit();
        subscription_align = this->create_subscription<sensor_msgs::msg::Image>(
                "/camera/aligned_depth_to_color/image_raw", 5, std::bind(&ModelBasedVisualServo::aligndepth_callback, this, std::placeholders::_1));
        subscription_color = this->create_subscription<sensor_msgs::msg::Image>(
                "/camera/color/image_raw", 5, std::bind(&ModelBasedVisualServo::color_callback, this, std::placeholders::_1));
        subscription_encoderdepth = this->create_subscription<std_msgs::msg::String>(
                "EncoderDepth", 5, std::bind(&ModelBasedVisualServo::encoderdepth_callback, this, std::placeholders::_1));
        publisher_ = this->create_publisher<std_msgs::msg::String>("VisualservoOutput", 5);
    }


    void setColotImage(const std::string picture_name)
    {
        color_image = cv::imread(picture_name);
    }

    void Calculate()
    {
        getFeaturePoint();
        if(squares.size() == 1 || squares.size() == 2)
        {
            std::sort(squares.begin(),squares.end(), compSquareSize);
            std::sort(squares[0].begin(),squares[0].end(), compPointXUp);
            std::sort(squares[0].begin(),squares[0].begin()+2, compPointYUp);
            std::sort(squares[0].begin()+2,squares[0].end(), compPointYDown);
            for(size_t i = 0;i < 4;i++)
            {
                cv::putText(color_image,std::to_string(i),squares[0][i],cv::FONT_HERSHEY_PLAIN,2,cv::Scalar(255,0,0),2);
                std::cout<<"point"<<i<<":("<<squares[0][i].x<<","<<squares[0][i].y<<")"<<std::endl;
            }
            if(mode == RUN) {
                if(getVisualServoResult() == 1){
                    VisualServoPlot();
                }
                vsCommandPublish();
            }
            else if(mode == SIM_VISP) {
                if(getVisualServoResult() == 1){
                    getVisualServoSimVISP();
                }
            }

        }

    }
    ModeType getMode()
    {
        return mode;
    }
    cv::Mat getColorImage()
    {
        return color_image;
    }
    cv::Mat getAlignDepthImage()
    {
        return depth_image;
    }
private:
    ModeType mode;

    cv::Mat color_image;
    cv::Mat depth_image;
    std::vector<std::vector<cv::Point> > squares;

    vpServo task; // Visual servoing task
    std::vector<vpFeaturePoint> sd; //The desired point feature.
    std::vector<vpFeaturePoint> s;
    float encoder_depth;
    int vsreach;
    cv::Point3f objectd;
    cv::Point3f object;
    cv::Point2f thetaz0;
    cv::Point2f thetaz3;
    std::vector<vpPoint> point;
    vpColVector v;
    vpPlot run_plotter;
    vpImage<unsigned char> Iint;
    vpImage<unsigned char> Iext;
    vpDisplayX displayInt;
    vpDisplayX displayExt;
    vpProjectionDisplay externalview;



    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_color;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_align;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_encoderdepth;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

// 函数绘制图像中的所有四边形

    static void drawSquares(cv::Mat& image, const std::vector<std::vector<cv::Point> >& squares)
    {
        for( size_t i = 0; i < squares.size(); i++ )
        {

            circle(image,squares[i][0],10,cv::Scalar(0,0,255),3);
            circle(image,squares[i][1],10,cv::Scalar(0,0,255),3);
            circle(image,squares[i][2],10,cv::Scalar(0,0,255),3);
            circle(image,squares[i][3],10,cv::Scalar(0,0,255),3);
            cv::line(image,squares[i][0],squares[i][1],cv::Scalar(0,255,0),3);
            cv::line(image,squares[i][1],squares[i][2],cv::Scalar(0,255,0),3);
            cv::line(image,squares[i][2],squares[i][3],cv::Scalar(0,255,0),3);
            cv::line(image,squares[i][3],squares[i][0],cv::Scalar(0,255,0),3);
        }


    }
    void getFeaturePoint()
    {
        squares.clear();
        cv::Mat gray_img;
        cv::cvtColor(color_image,
                     gray_img,
                     cv::COLOR_BGR2GRAY);
        cv::blur(gray_img,gray_img,cv::Size(3,3));
//        cv::threshold(gray_img,gray_img,80,255,cv::THRESH_BINARY);
        Canny(gray_img, gray_img, 200, 300, 5);
        cv::imshow("gray",gray_img);

        //轮廓检测
        std::vector<std::vector<cv::Point> > contours;
        findContours(gray_img, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        //多边形检测gray_img
        std::vector<cv::Point> approx;
        //一次取一个轮廓判断下是不是矩形
        for( size_t i = 0; i < contours.size(); i++ )
        {

            //approx存储了近似后的轮廓
            approxPolyDP(cv::Mat(contours[i]), approx, arcLength(cv::Mat(contours[i]), true)*0.02, true);

            if( approx.size() == 4 && //矩形必须是四个点
                fabs(contourArea(cv::Mat(approx))) > 3000 &&
                isContourConvex(cv::Mat(approx)) )
            {
                std::cout<<"The square's size is "<<fabs(contourArea(cv::Mat(approx)))<<std::endl;
                squares.push_back(approx);

            }
        }
        std::cout<<"Find "<<squares.size()<<" squares."<<std::endl;


        drawSquares(color_image,squares);
    }

    void project2ImagePlane(float& x,float & y)
    { //focal is like (x - 611.075)/610.372*1.88
        x = (x - 322.86)/610.372;
        y = (y - 238.664)/611.075;
    }
    void VisualServoInit()
    {
        task.setServo(vpServo::EYEINHAND_CAMERA);
        task.setInteractionMatrixType(vpServo::CURRENT);
        task.setLambda(0.5);
        sd.resize(4);
        s.resize(4);
        encoder_depth = 0;
        v.resize(6,1,true);
        vsreach = 0;
        object.x = 0;object.y = 0;object.z = 0;
        objectd.x = 0;objectd.y = 0;objectd.z = 0;
        thetaz0.x = 0;thetaz0.y = 0;
        thetaz3.x = 0;thetaz3.y = 0;
        float x,y,z;

        x = 222;
        y = 126;
        z = 0.169;
        project2ImagePlane(x,y);
        objectd.x += x * z;objectd.y += y * z;objectd.z += z;
        sd[0].buildFrom(x, y, z);
        x = 224;
        y = 339;
        z = 0.169;
        project2ImagePlane(x,y);
        objectd.x += x * z;objectd.y += y * z;objectd.z += z;
        sd[1].buildFrom(x, y, z);
        x = 436;
        y = 339;
        z = 0.169;
        project2ImagePlane(x,y);
        objectd.x += x * z;objectd.y += y * z;objectd.z += z;
        sd[2].buildFrom(x, y, z);
        x = 436;
        y = 124;
        z = 0.169;
        project2ImagePlane(x,y);
        objectd.x += x * z;objectd.y += y * z;objectd.z += z;
        sd[3].buildFrom(x, y, z);
        objectd.x /= 4;objectd.y /= 4;objectd.z /= 4;
        for(size_t i = 0; i<4;i++)
        {
            point.emplace_back(vpPoint(sd[i].get_x()*sd[i].get_Z() - objectd.x,sd[i].get_y()*sd[i].get_Z() - objectd.y,sd[i].get_Z() - objectd.z));
        }
        for(size_t i = 0;i < 4 ;i++)
        {
            task.addFeature(s[i], sd[i]);
        }

        //run plotter init
        run_plotter.init(2, 500 * 2, 1000, 730, 400, "Real time curves plotter");

        run_plotter.setTitle(0, "Visual features error");
        run_plotter.setTitle(1, "Camera velocities");

        run_plotter.initGraph(0, 8);
        run_plotter.initGraph(1, 6);
        run_plotter.setGraphThickness(0,2);
        run_plotter.setGraphThickness(1,2);
        run_plotter.setGridThickness(0,2);
        run_plotter.setGridThickness(1,2);
        run_plotter.setLegend(0, 0, "x1");
        run_plotter.setLegend(0, 1, "y1");
        run_plotter.setLegend(0, 2, "x2");
        run_plotter.setLegend(0, 3, "y2");
        run_plotter.setLegend(0, 4, "x3");
        run_plotter.setLegend(0, 5, "y3");
        run_plotter.setLegend(0, 6, "x4");
        run_plotter.setLegend(0, 7, "y4");
//怎么又输入不了中文？
        run_plotter.setLegend(1, 0, "v_x");
        run_plotter.setLegend(1, 1, "v_y");
        run_plotter.setLegend(1, 2, "v_z");
        run_plotter.setLegend(1, 3, "w_x");
        run_plotter.setLegend(1, 4, "w_y");
        run_plotter.setLegend(1, 5, "w_z");
        //sim cam init
        Iint.init(480, 640, 255);
        Iext.init(480, 640, 255);
        displayInt.init(Iint, 0, 0, "Internal view");
        displayExt.init(Iext, 0, 500, "External view");
        for (unsigned int i = 0; i < 4; i++)
            externalview.insert(point[i]);

    }
    //run
//    vpColVector MyVSControlLaw(){
//        static int init_e0 = 0;
//        static float eric = 0;
//        static vpColVector e0(8,0);
//        vpMatrix L = task.computeInteractionMatrix();
//        vpColVector e = task.computeError();
//        vpMatrix L_inverse = LAMDA_RUN * L.AtA().inverseByLU() * L.transpose();
//        vpColVector cal_v;
//        if(init_e0 < 100)
//        {
//            init_e0++;
//            e0 = e0 + e;
//            cal_v.resize(6,1,true);
//            if(init_e0 == 100)
//            {
//                e0 = e0 / 100;
//
//            }
//
//        }
//        else if(init_e0 >= 100)
//        {
//
//            cal_v = - L_inverse * e - L_inverse * e0 * exp(- 2.3 * eric * 0.033);
//            eric = eric + 1;
//
//        }
//
//
//    if(this->mode == RUN) {
//        cal_v[0] *= 100 *0.9;
//        cal_v[1] *= 100 *0.37;
//        cal_v[2] *= 100 *0.4;
////        cal_v[1] = (cal_v[1] + 0.4 * v[3]) * 100;
//        cal_v[3] = 0;
//        cal_v[4] = 0;
//        cal_v[5] *= 180 / 3.1415926 * 0.5;
//
//    }
//        return cal_v;
//    }
    vpColVector MyVSControlLaw(){
        static int init_e0 = 0;
        static float eric = 0;
        static vpColVector e0(8,0);
        static vpColVector minus_coeffication(8,0);

        vpColVector e = task.computeError();

        vpColVector cal_v;
        if(init_e0 < 100)
        {
            init_e0++;
            e0 = e0 + e;
            cal_v.resize(6,1,true);
            if(init_e0 == 100)
            {
                e0 = e0 / 100;
                double min_e_val = vpColVector::sort(e0)[0];
                for(size_t i = 0;i < 8;i++)
                {
                    minus_coeffication[i] = exp(0.8*(std::abs(e0[i]/min_e_val)-1));
                }

            }

        }
        else if(init_e0 >= 100)
        {
            vpMatrix L = task.computeInteractionMatrix();
            vpMatrix A;

            for(size_t i = 0;i < 8;i++)
            {
                A = vpMatrix::stack(A,L.getRow(i) * minus_coeffication[i]);
            }
            vpMatrix L_inverse = LAMDA_RUN * L.AtA().inverseByLU() * L.transpose();
            cal_v =  - L_inverse * e -  L_inverse * e0 * exp(- 2.3  * eric * 0.033);
            eric = eric + 1;

        }


        if(this->mode == RUN) {
            cal_v[0] *= 100 *0.9;
            cal_v[1] *= 100 *0.37;
            cal_v[2] *= 100 *0.4;
//        cal_v[1] = (cal_v[1] + 0.4 * v[3]) * 100;
            cal_v[3] = 0;
            cal_v[4] = 0;
            cal_v[5] *= 180 / 3.1415926 * 0.5;

        }
        return cal_v;
    }
    void VisualServoPlot()
    {
        static int iter = 0;
        static vpCameraParameters cam(1000, 1000, this->Iint.getWidth() / 2, this->Iint.getHeight() / 2);
        static vpHomogeneousMatrix cextMo(0, 0, 1, 0, 0, 0);
        static std::vector<vpImagePoint> traj[4];
        run_plotter.plot(0, iter, this->task.getError());
        run_plotter.plot(1, iter, this->v);
        vpHomogeneousMatrix cMo(object.x, object.y, object.z, 0, 0, atan2(thetaz3.y - thetaz0.y,thetaz3.x - thetaz0.y));
        vpDisplay::display(Iint);
        vpDisplay::display(Iext);
//        display_trajectory(Iint, point, cMo, cam);


        vpImagePoint cog;
        for (unsigned int i = 0; i < 4; i++) {
            vpMeterPixelConversion::convertPoint(cam, s[i].get_x(), s[i].get_y(), cog);
            traj[i].push_back(cog);
        }
        for (unsigned int i = 0; i < 4; i++) {
            for (unsigned int j = 1; j < traj[i].size(); j++) {
                vpDisplay::displayLine(Iint, traj[i][j - 1], traj[i][j], vpColor::green);
            }
        }

        vpServoDisplay::display(task, cam, Iint, vpColor::green, vpColor::red);
        vpDisplay::flush(Iint);
        if(this->mode == RUN) {
            externalview.display(Iext, cextMo, cMo, cam, vpColor::red, true);
            vpDisplay::flush(Iext);
        }

        iter++;
    } //plot for run
    int getVisualServoResult()
    {
        static float delta_z;
        float x,y,z;
        object.x = 0;object.z = 0;object.z = 0;
        for(size_t i = 0;i < 4 ;i++)
        {
            x = squares[0][i].x;
            y = squares[0][i].y;
            cv::medianBlur(depth_image,depth_image,5);
            if (vsreach == 0){
                z = double(depth_image.at<cv::uint16_t>(y,x))*0.001;
                delta_z = encoder_depth - z;
            }
            else
            {
                z = encoder_depth - delta_z;
            }
            std::cout<<"z:"<<z<<std::endl;
            if(z == 0)
            {
                std::cout<<"There is a z equal to 0!"<<std::endl;

                return 0;
            }
            project2ImagePlane(x,y);
            s[i].buildFrom(x, y, z);
            object.x += x * z;
            object.y += y * z;
            object.z += z;
            if(i == 0)
            {
                thetaz0.x = x;thetaz0.y = y;
            }
            else if(i == 3)
            {
                thetaz3.x = x;thetaz3.y = y;
            }
        }
        object.x /= 4;
        object.y /= 4;
        object.z /= 4;
        if(object.z < 0.25)
        {
            vsreach = 1;
            std::cout<<"Disable the camera depth.\nTurn to encoder mode."<<std::endl;
        }
        else
        {
            vsreach = 0;
            std::cout<<"Disable the encoder depth.\nTurn to camera mode."<<std::endl;
        }
        v = MyVSControlLaw();
        return 1;
//        v = task.computeControlLaw(); // camera velocity
    }
    void vsCommandPublish(){
        std::stringstream ss;
        ss<<"[";

        for(size_t i = 0;i < 6;i++)
        {
            std::cout<<v[i]<<" ";
            ss<<v[i];
//            if(i != 5)
            ss<<",";
//            else
//                ss<<"]";

        }
        ss<<vsreach<<"]";
        std::cout<<vsreach<<std::endl;
//        std::cout<<std::endl;
        auto message = std_msgs::msg::String();
        message.data = ss.str();
        publisher_->publish(message);
    }
    //这就是个画线的程序
    void display_trajectory(const vpImage<unsigned char> &I, std::vector<vpPoint> &point, const vpHomogeneousMatrix &cMo,
                            const vpCameraParameters &cam)
    {
        static std::vector<vpImagePoint> traj[4];
        vpImagePoint cog;
        for (unsigned int i = 0; i < 4; i++) {
            // Project the point at the given camera position
            point[i].track(cMo);
            vpMeterPixelConversion::convertPoint(cam, point[i].get_x(), point[i].get_y(), cog);
            traj[i].push_back(cog);
        }
        for (unsigned int i = 0; i < 4; i++) {
            for (unsigned int j = 1; j < traj[i].size(); j++) {
                vpDisplay::displayLine(I, traj[i][j - 1], traj[i][j], vpColor::green);
            }
        }
    }
    void getVisualServoSimVISP(){
        //初始化仿真变量
        vpHomogeneousMatrix cdMo(0, 0, objectd.z, 0, 0, 0);
        vpHomogeneousMatrix cMo(object.x, object.y, object.z, 0, 0, atan2(thetaz3.y - thetaz0.y,thetaz3.x - thetaz0.y));

        for (unsigned int i = 0; i < 4; i++) {
            point[i].track(cMo);
            vpFeatureBuilder::create(s[i], point[i]);
        }

        vpHomogeneousMatrix wMc, wMo;
        vpSimulatorCamera robot;
        robot.setSamplingTime(0.040);
        robot.getPosition(wMc);
        wMo = wMc * cMo;

        vpCameraParameters cam(1000, 1000, this->Iint.getWidth() / 2, this->Iint.getHeight() / 2);
        vpHomogeneousMatrix cextMo(0, 0, 1, 0, 0, 0);
        unsigned int iter = 0;

        while (1) {
            robot.getPosition(wMc);
            cMo = wMc.inverse() * wMo;
            for (unsigned int i = 0; i < 4; i++) {
                point[i].track(cMo);
                vpFeatureBuilder::create(s[i], point[i]);
            }
            vpColVector vel = MyVSControlLaw();
//            vpColVector v = task.computeControlLaw();
            float bak = vel[3];
            vel[3] = 0;vel[4] = 0;
            for(size_t i = 0;i < 6;i++)
            {
                this->v[i] = vel[i];
            }

            robot.setVelocity(vpRobot::CAMERA_FRAME, vel);
            VisualServoPlot();
            externalview.display(Iext, cextMo, cMo, cam, vpColor::red, true);
            vpDisplay::flush(Iext);
            vel[5] *= 180 / 3.1415926;
            vel[0] *= 100;vel[1] = (vel[1] - bak * 0.2) * 100;
            vel[2] *= 100;


            //收敛条件
            if ((task.getError()).sumSquare() < 0.0001)
                break;

            // A click to exit
            if (vpDisplay::getClick(Iint, false) || vpDisplay::getClick(Iext, false))
                break;

            vpTime::wait(robot.getSamplingTime() * 1000);
        }
        task.kill();
        while(1){}
    }
    //subscriber callback
    void aligndepth_callback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "I heard ALIGN: %d",msg->data[500]);
        depth_image = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_16UC1)->image;
        visualready.setAlignOK(true);
    }
    void color_callback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "I heard COLOR: %s",msg->encoding.c_str());
        color_image = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;
        visualready.setColorOK(true);
    }
    void encoderdepth_callback(const std_msgs::msg::String::SharedPtr msg){
        RCLCPP_INFO(this->get_logger(), "I heard EncoderDepth: %s",msg->data.c_str());
        encoder_depth = std::atof(msg->data.c_str());
    }
};

int main(int argc, char * argv[])
{


    rclcpp::init(argc, argv);
    //parameter for colorimg
    cv::Mat color_image;
    cv::Mat depth_image;
    std::string OPENCV_WINDOW = "color_image";
    std::string OPENCV_DEPTH_WINDOW = "depth_image";
    cv::namedWindow(OPENCV_WINDOW);
    //实时获取相机深度图对应的点云
    std::shared_ptr<ModelBasedVisualServo> PhotCaptureTool =
            std::make_shared<ModelBasedVisualServo> (argc, argv);

    // param in sim_visp mode
    int picture_ignore_num = 0;
    int picture_ignore_maxnum = 20;
    while(rclcpp::ok()){

//
    switch(PhotCaptureTool->getMode())
    {
        case ModelBasedVisualServo::RUN:
            rclcpp::spin_some(PhotCaptureTool);

            if(visualready.getVSOK() == true)
            {

                visualready.refreshOK();
                PhotCaptureTool->Calculate();
                cv::imshow(OPENCV_WINDOW, PhotCaptureTool->getColorImage());
                cv::waitKey(3);
            }
            break;
        case ModelBasedVisualServo::SIM_VISP:
            rclcpp::spin_some(PhotCaptureTool);
            if(visualready.getVSOK() == true)
            {
                visualready.refreshOK();
                if(picture_ignore_num < picture_ignore_maxnum)
                    picture_ignore_num++;
                else
                {
                    PhotCaptureTool->Calculate();
                }
                cv::imshow(OPENCV_WINDOW, PhotCaptureTool->getColorImage());
                cv::waitKey(3);
            }
            break;
        default:
            std::cout<<"Wrong Mode"<<std::endl;
    }



    }


    rclcpp::shutdown();
    return 0;

}



//int main1(int argc, char * argv[])
//{
//    cv::Mat color_image;
//    color_image = cv::imread("/home/wzf/main_folder/CLionProjects/JK_project/src/realsense/data/apritag/tag36_11_00000_page-0001.jpg");
////    cv::imshow("test",color_image);
////    std::cout<<color_image.size<<std::endl;
//    //图像灰度化 边缘检测
//    cv::Mat gray_img;
//    cv::cvtColor(color_image,
//                 gray_img,
//                 cv::COLOR_BGR2GRAY);
//    Canny(gray_img, gray_img, 50, 100, 5);
//
//
//    //轮廓检测
//    std::vector<std::vector<cv::Point> > contours;
//    findContours(gray_img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
//
//    //多边形检测
//    std::vector<cv::Point> approx;
//    std::vector<std::vector<cv::Point> > squares;
//    //一次取一个轮廓判断下是不是矩形
//    for( size_t i = 0; i < contours.size(); i++ )
//    {
//
//        //approx存储了近似后的轮廓
//        approxPolyDP(cv::Mat(contours[i]), approx, arcLength(cv::Mat(contours[i]), true)*0.02, true);
//
//        if( approx.size() == 4 && //矩形必须是四个点
//            fabs(contourArea(cv::Mat(approx))) > 10000 &&
//            isContourConvex(cv::Mat(approx)) )
//        {
//             std::cout<<fabs(contourArea(cv::Mat(approx)))<<std::endl;
//             squares.push_back(approx);
//
//        }
//    }
//    std::cout<<squares.size()<<std::endl;
////    drawSquares(color_image,squares);
////    cv::imshow("test",gray_img);
////    cv::waitKey(0);
//
//
//
//    return 0;
//
//
//}