#include <memory>
#include <string>
#include <iostream>
#include <list>
//ros
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <image_transport/image_transport.hpp>
#include <cv_bridge/cv_bridge.h>
//realsense
#include <librealsense2/rs.hpp>
//pcl
#include <pcl/console/print.h>
//opencv
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/photo/photo.hpp>
//eigen
#include <eigen3/Eigen/Dense>
//visp
#include <visp3/visual_features/vpFeatureBuilder.h>
#include <visp3/vs/vpServo.h>
#include <visp3/robot/vpSimulatorCamera.h>
#include <visp3/gui/vpDisplayX.h>
#include <visp3/gui/vpProjectionDisplay.h>
#include <visp3/vs/vpServoDisplay.h>
#include <visp3/gui/vpPlot.h>
#include <visp3/core/vpMatrix.h>
using namespace std;

//小模块类
class VSReady
{
private:
    bool AlignOK;
    bool ColorOK;
    bool GraspOk;
    bool VSOK;
public:
    VSReady()
    {
        AlignOK = false;
        ColorOK = false;
        GraspOk = false;
        VSOK = false;
    }
    void setAlignOK(bool setflag)
    {
        this->AlignOK = setflag;
        if(this->ColorOK == true && this->GraspOk == true)
            this->VSOK = true;
    }
    void setColorOK(bool setflag)
    {
        this->ColorOK = setflag;
        if(this->AlignOK == true && this->GraspOk == true)
            this->VSOK = true;
    }
    void setGraspOK(bool setflag)
    {
        this->GraspOk = setflag;
        if(this->AlignOK == true && this->ColorOK == true)
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
        GraspOk = false;
        VSOK = false;
    }
    ~VSReady(){}
};
class Grasp_Data{
public:
    Grasp_Data(){
        this->x = 0;this->y = 0;this->theta = 0;this->width = 0;
    }
    ~Grasp_Data(){}
    void showGraspData(){
        cout<<"x:"<<this->x<<"y:"<<this->y<<"theta:"<<this->theta<<"width:"<<this->width<<endl;
    }
    float& operator[](const int num){
        switch (num) {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return theta;
            case 3:
                return width;
            default:
                throw "Grasp_Data: Out Of Range!";
        }
    }


private:
    float x;
    float y;
    float theta;
    float width;
    friend class GraspDataHandle;
    friend class GGCNNBasedVisualServo;
    friend class SquareFeature2D;
    friend class GraspFeature2D;
    friend class NetGraspFeature2D;
} ;
class KalmanFilter
{
public:
    KalmanFilter(int stateSize_, int measSize_,int uSize_){
        this->stateSize = stateSize_;
        this->measSize = measSize_;
        this->uSize = uSize_;
        if (stateSize == 0 || measSize == 0)
        {
            std::cerr << "Error, State size and measurement size must bigger than 0\n";
        }

        x.resize(stateSize);
        x.setZero();

        A.resize(stateSize, stateSize);
        A.setIdentity();

        u.resize(uSize);
        u.transpose();
        u.setZero();

        B.resize(stateSize, uSize);
        B.setZero();

        P.resize(stateSize, stateSize);
        P.setIdentity();

        H.resize(measSize, stateSize);
        H.setZero();

        z.resize(measSize);
        z.setZero();

        Q.resize(stateSize, stateSize);
        Q.setZero();

        R.resize(measSize, measSize);
        R.setZero();
    }
    KalmanFilter(){}
    ~KalmanFilter(){}
    void init(Eigen::VectorXd &x_, Eigen::MatrixXd& P_,Eigen::MatrixXd& R_, Eigen::MatrixXd& Q_){
        x = x_;
        P = P_;
        R = R_;
        Q = Q_;
    }
    Eigen::VectorXd predict(Eigen::MatrixXd& A_, Eigen::MatrixXd &B_, Eigen::VectorXd &u_)
    {
        A = A_;
        B = B_;
        u = u_;
        x = A*x + B*u;
        Eigen::MatrixXd A_T = A.transpose();
        P = A*P*A_T + Q;
        return x;
    }
    Eigen::VectorXd predict(Eigen::MatrixXd& A_)
    {
        A = A_;
        x = A*x;
        Eigen::MatrixXd A_T = A.transpose();
        P = A*P*A_T + Q;
//  cout << "P-=" << P<< endl;
        return x;
    }

    void update(Eigen::MatrixXd& H_,Eigen::VectorXd z_meas){
        H = H_;
        Eigen::MatrixXd temp1, temp2,Ht;
        Ht = H.transpose();
        temp1 = H*P*Ht + R;
        temp2 = temp1.inverse();//(H*P*H'+R)^(-1)
        Eigen::MatrixXd K = P*Ht*temp2;
        z = H*x;
        x = x + K*(z_meas-z);
        Eigen::MatrixXd I = Eigen::MatrixXd::Identity(4, 4);
        P = (I - K*H)*P;
//  cout << "P=" << P << endl;
    }

    void filter_start(Eigen::VectorXd observe_state, Grasp_Data& filtered_data, float delta_time)
    {
        static bool if_init = false;
        if(if_init == false){
            P.setIdentity();
            Q.setIdentity()*1;
            R.setIdentity();
            this->init(observe_state,P,R,Q);
            filtered_data[0] = observe_state[0];
            filtered_data[1] = observe_state[1];
            if_init = true;
        }
        else{
            A << 1, 0, delta_time, 0,
                    0, 1, 0, delta_time,
                    0, 0, 1, 0,
                    0, 0, 0, 1;
            this->predict(A);
            z<<observe_state[0],observe_state[1];
            H<<1,0,0,0,
                    0,1,0,0;
            this->update(H,z);
            cout<<x<<endl;
            filtered_data[0] = x[0];
            filtered_data[1] = x[1];
        }
    }

private:
    int stateSize; //state variable's dimenssion
    int measSize; //measurement variable's dimession
    int uSize; //control variables's dimenssion
    Eigen::VectorXd x;
    Eigen::VectorXd z;
    Eigen::MatrixXd A;
    Eigen::MatrixXd B;
    Eigen::VectorXd u;
    Eigen::MatrixXd P;//coveriance
    Eigen::MatrixXd H;
    Eigen::MatrixXd R;//measurement noise covariance
    Eigen::MatrixXd Q;//process noise covariance
};
class EncoderDepthHandle{
public:
    EncoderDepthHandle(){
        this->delta_depth = 0;
    }
    ~EncoderDepthHandle(){}
    void setDeltaDepth(float encoder_depth,float target_depth){
        this->delta_depth = encoder_depth - target_depth;
    }
    float getDepth(float encoder_depth){
        return (encoder_depth - this->delta_depth);
    }
private:
    float delta_depth;
};
class RunningState{
public:
    RunningState(){
        this->if_refresh_target_point = true;
        this->running_mode = Tracking;
        this->depth_get_mode = Camera;
    }
    ~RunningState(){}
    enum RunningMode{
        Tracking,
        Grasp,
        Stop
    };
    enum DepthGetMode{
        Camera,
        Encoding
    };
    void setMode(float target_depth, float error){
        if(target_depth > 0.3)
        {
            if_refresh_target_point = true;
            depth_get_mode = Camera;
            cout<<target_depth<<" refresh,Camera,";
        }
        else if(target_depth <= 0.3 && target_depth >0.2)
        {
            if_refresh_target_point = false;
            depth_get_mode = Camera;
            cout<<target_depth<<" norefresh,Camera,";
        }
        else if(target_depth <= 0.2)
        {
            if_refresh_target_point = false;
            depth_get_mode = Encoding;
            cout<<target_depth<<" norefresh,Encoding,";
        }
        if(error <= 0.1) {
            running_mode = Grasp;
            cout<<"Grasp"<<endl;
        }
        else {
            running_mode = Tracking;
            cout<<"Tracking"<<endl;
        }
    }
    void setModePlus(float font_depth, float error){
        if(font_depth > 0.2)
        {
            if_refresh_target_point = true;
            running_mode = Tracking;
            depth_get_mode = Camera;
            cout<<font_depth<<" Tracking,Refresh,Camera,";
        }
        else if(font_depth <= 0.2)
        {
            if_refresh_target_point = false;
            running_mode = Stop;
            depth_get_mode = Camera;
            cout<<font_depth<<" Stop,NoRefresh,Camera,";
        }
        if(font_depth < 0.3 && error < 2 * 0.00001)
        {
            if_refresh_target_point = false;
            running_mode = Stop;
            depth_get_mode = Camera;
            cout<<"error:"<<error<<endl;
        }

    }
    bool if_refresh_target_point;
    RunningMode running_mode;
    DepthGetMode depth_get_mode;
private:
};
VSReady visualready;

//用于处理抓取点数据，使之表现的更加平稳
class GraspDataHandle{
public:


    GraspDataHandle(){
        this->grasp_data = Grasp_Data();
        this->grasp_data_filtered = Grasp_Data();
        this->delta_time = 0.03;

    }
    ~GraspDataHandle(){}
    float getFontDepth(){
        return this->font_depth;
    }

    void setGraspData(const string& msg){
        if(*msg.begin() == '[' && *(msg.end()-1) == ']') {
            string s1(msg.begin()+1,msg.end()-1),s2;
            stringstream ss(s1);int num = 0;
            while(getline(ss, s2, ',')) {
                if(num < 4) {
                    this->grasp_data[num] = std::atof(s2.c_str());

                }
                else if(num == 4)
                {
                    font_depth = std::atof(s2.c_str());
                    font_depth = this->fontDepthFilter(font_depth,0.8,5);
                }
                else if(num == 5)
                {
                    back_depth = std::atof(s2.c_str());
                }
                num++;
            }
        }
        else{
            cout <<"Error MSG Of Grasp!"<<endl;
            return ;
        }
    }
    void handleGraspData(float learning_rate){
//        Eigen::VectorXd x(4);x<<grasp_data.x,grasp_data.y,0,0;
//        this->kalman_filter_tool.filter_start(x, grasp_data_filtered, this->delta_time);
//        this->grasp_data_filtered.x = this->grasp_data.x;
//        this->grasp_data_filtered.y = this->grasp_data.y;
//        this->grasp_data_filtered.theta = this->grasp_data.theta;
//        this->grasp_data_filtered.width = this->grasp_data.width;
        this->dataSmooth(grasp_data,grasp_data_filtered,learning_rate,5);
    }
    void showGraspData(){
            cout<<"x:"<<grasp_data.x<<"y:"<<grasp_data.y<<"theta:"<<grasp_data.theta<<"width:"<<grasp_data.width<<endl;
    }
    void dataSmooth(Grasp_Data input,Grasp_Data& output,float learning_rate,int average_num){
        static int init_num = 0;
        for(size_t i = 0;i < 4;i++){
            if(init_num < average_num)
            {
                output[i] += input[i]/float(average_num);
                init_num++;
            }
            else
            {
                output[i] = (1 - learning_rate) * output[i] + learning_rate * input[i];
            }

        }
    }
private:
    Grasp_Data grasp_data;
    Grasp_Data grasp_data_filtered;
    float font_depth;
    float back_depth;
    float delta_time;
    KalmanFilter kalman_filter_tool = KalmanFilter(4,2,0);
    friend class GGCNNBasedVisualServo;
    float fontDepthFilter(float input,float learning_rate,int average_num){
        static int init_num = 0;
        static float output = 0;

        if(init_num < average_num)
        {
            output += input/float(average_num);
            init_num++;
            return input;
        }
        else
        {
            output = (1 - learning_rate) * output + learning_rate * input;
            return output;
        }


    }

};
//用于获取图片特征
bool compSquareSize(std::vector<cv::Point> S1,std::vector<cv::Point> S2)
{
    return fabs(contourArea(cv::Mat(S1))) < fabs(contourArea(cv::Mat(S2)));
}
class SquareFeature2D{
public:
    SquareFeature2D(){
        feature_points.resize(4);
        delta_depth.resize(4);
        T.resize(2);
        R.resize(2,2);
        target_feature_points.resize(4);
        detect_feature_points.resize(4);
        for(size_t i = 0;i < 4;i++){
            target_feature_points[i].resize(2);
            detect_feature_points[i].resize(2);
        }
    }
    ~SquareFeature2D(){}
    void drawSquares(cv::Mat& image, const std::vector<std::vector<cv::Point> >& squares)
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
    bool getFeaturePoint(cv::Mat color_image)
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
                fabs(contourArea(cv::Mat(approx))) > 1000 &&
                isContourConvex(cv::Mat(approx)) )
            {
                std::cout<<"The square's size is "<<fabs(contourArea(cv::Mat(approx)))<<std::endl;
                squares.push_back(approx);

            }
        }
        std::cout<<"Find "<<squares.size()<<" squares."<<std::endl;


        drawSquares(color_image,squares);

        if(squares.size() == 1 || squares.size() == 2)
        {
            std::sort(squares.begin(),squares.end(), compSquareSize);
            for(size_t i = 0;i < 4;i++)
            {
                cv::putText(color_image,std::to_string(i),squares[0][i],cv::FONT_HERSHEY_PLAIN,2,cv::Scalar(255,0,0),2);
                std::cout<<"point"<<i<<":("<<squares[0][i].x<<","<<squares[0][i].y<<")"<<std::endl;
                feature_points[i].x = (float)squares[0][i].x;feature_points[i].y = (float)squares[0][i].y;
            }
            return true;
        }
        else
        {
            cout<<"Squares Size Out of Range!"<<endl;
            return false;
        }
    }
    bool refreshFeaturePoint(cv::Mat depth_image, float encoder_depth){
//        cv2.inpaint(img, (img==0).astype(np.uint8), 3, cv2.INPAINT_NS)
        for(size_t i = 0;i < 4;i++)
        {
            feature_points[i].z = float(depth_image.at<cv::uint16_t>(ceil(feature_points[i].y),ceil(feature_points[i].x)))*0.001;
            if(feature_points[i].z == 0)
            {
                cout<<"There is z = 0!"<<endl;
                return false;
            }
            delta_depth[i].setDeltaDepth(encoder_depth, feature_points[i].z);
        }
        return true;
    }
    bool refreshFeaturePoint(float encoder_depth){
        for(size_t i = 0;i < 4;i++)
        {
            feature_points[i].z = delta_depth[i].getDepth(encoder_depth);
            if(feature_points[i].z == 0)
            {
                cout<<"There is z = 0!"<<endl;
                return false;
            }
        }
        return true;
    }
    float getTargetDepth()
    {
        float sum = 0;
        for(size_t i = 0;i < 4;i++)
        {
            sum += feature_points[i].z;
        }
        this->average_feature_depth = (sum)/4;
        return this->average_feature_depth;
    }
    void regreshTargetFeaturePoint(Grasp_Data grasp_point_filtered)
    {
        for(size_t i = 0;i < 4;i++){
            T<<-grasp_point_filtered.x,-grasp_point_filtered.y;
            R<<cos(grasp_point_filtered.theta - CV_PI/2),-sin(grasp_point_filtered.theta - CV_PI/2),
               sin(grasp_point_filtered.theta - CV_PI/2),cos(grasp_point_filtered.theta - CV_PI/2);
            detect_feature_points[i]<<feature_points[i].x,feature_points[i].y;
            target_feature_points[i] = average_feature_depth/0.25*(R*(detect_feature_points[i]+T));
        }
    }
    void drawTargetFeaturePoint(cv::Mat img2draw)
    {
        vector<cv::Point2i> target_feature_output(4);
        for(size_t i = 0;i < 4;i++){
            target_feature_output[i].x = (int)(target_feature_points[i][0] + 320);
            target_feature_output[i].y = (int)(target_feature_points[i][1] + 240);
            cout<<"target_feature_x"<<target_feature_output[i].x<<"\ntarget_feature_y"<<target_feature_output[i].y<<endl;
            cv::circle(img2draw, target_feature_output[i], 5, cv::Scalar(255,0,0),2);
        }
        cv::imshow("SquareTargetPoints", img2draw);
//        cv::waitKey(1);
    }
    std_msgs::msg::String getSpeedCommand()
    {
        auto message = std_msgs::msg::String();
        vpServo task; // Visual servoing task
        float x,y,z;sd.resize(4);s.resize(4);

        for(int i = 0;i < sd.size();i++){
            x = target_feature_points[i][0] + 320;
            y = target_feature_points[i][1] + 240;
            z = feature_points[i].z;
            project2CameraPlane(x,y,0.3);
            sd[i].buildFrom(x, y, 0.3);
            x = feature_points[i].x;y = feature_points[i].y;z = feature_points[i].z;
            project2CameraPlane(x,y,z);
            s[i].buildFrom(x, y, z);
            task.addFeature(s[i], sd[i]);
        }
        vpColVector e = task.computeError();
        vpMatrix L = task.computeInteractionMatrix();
        vpMatrix L_inverse = 0.25 * L.AtA().inverseByLU() * L.transpose();
        v =  - L_inverse * e;
        v[0] *= 50;
//        v[1] = (v[1] + 0.4 * v[3]) * 200;
        v[1] *= 50;
        v[2] *= 50;
        v[5] *= 180 / 3.1415926 * 0.1;
//        v[5] = 0;
        v = dataSmooth(v, 10, 0.05);
        message.data = "["+ to_string(v[0]) + ',' + to_string(v[1]) + ','+ to_string(v[2]) + ','+ to_string(v[5]) + ']';;


//        message.data = "["+ to_string(v) + ',' + to_string(v+1) + ','+ to_string(v+2) + ','+ to_string(v+3) + ']';



        return message;
    }
private:

    void project2CameraPlane(float& x,float & y,float z)
    { //focal is like (x - 611.075)/610.372*1.88
        x = z * (x - 322.86)/610.372;
        y = z * (y - 238.664)/611.075;
    }
    vpColVector dataSmooth(vpColVector input, int average_num, float learning_rate){
        static int init_num = 0;
        static vpColVector v_filtered(6,0);
        if(init_num < average_num)
        {
            v_filtered += input/float(average_num);
            init_num++;
            return input;
        }
        else
        {
            v_filtered = (1 - learning_rate) * v_filtered + learning_rate * input;
            return v_filtered;
        }

    }
//    特征处理
    vector<vector<cv::Point> > squares;
    vector<cv::Point3f> feature_points;
    vector<EncoderDepthHandle> delta_depth;
//    目标特征点获取
    float average_feature_depth;
    Eigen::VectorXd T;
    Eigen::MatrixXd R;
    vector<Eigen::VectorXd> detect_feature_points;
    vector<Eigen::VectorXd> target_feature_points;
    float target_feature_depth;
//    视觉伺服

    std::vector<vpFeaturePoint> sd; //The desired point feature.
    std::vector<vpFeaturePoint> s;
    vpColVector v;

//  友元类
    friend class GGCNNBasedVisualServo;
};
bool CompareMatch(cv::DMatch m1, cv::DMatch m2)
{
    return m1.distance < m2.distance;
}

class GraspFeature2D{
public:
    GraspFeature2D(){
        feature_points.resize(4);
        T.resize(2);
        R.resize(2,2);
        target_feature_points.resize(4);
        detect_feature_points.resize(4);
        for(size_t i = 0;i < 4;i++){
            target_feature_points[i].resize(2);
            detect_feature_points[i].resize(2);
        }
        sd.resize(4);s.resize(4);

    }
    ~GraspFeature2D(){}
    void drawSquares(cv::Mat& img2draw)
    {
        for( size_t i = 0; i < 4; i++ )
        {

            circle(img2draw,cv::Point2f(feature_points[i].x,feature_points[i].y),10,cv::Scalar(0,0,255),3);
//            circle(img2draw,cv::Point2f(sd[i].get_x(),sd[i].get_y()),10,cv::Scalar(0,255,0),3);
//            float x = sd[i].get_x();float y = sd[i].get_y();
//            cv::putText(img2draw,to_string(i),cv::Point2f(x,y),cv::FONT_HERSHEY_PLAIN,2,cv::Scalar(255,0,0),2);
//            project2CameraPlane(x,y,0.25);
//            sd[i].set_xyZ(x,y,0.25);
            cv::putText(img2draw,to_string(i),cv::Point2f(feature_points[i].x,feature_points[i].y),cv::FONT_HERSHEY_PLAIN,2,cv::Scalar(255,0,0),2);

        }
        cv::line(img2draw,cv::Point2f(feature_points[0].x,feature_points[0].y),cv::Point2f(feature_points[1].x,feature_points[1].y),cv::Scalar(0,255,0),3);
        cv::line(img2draw,cv::Point2f(feature_points[1].x,feature_points[1].y),cv::Point2f(feature_points[2].x,feature_points[2].y),cv::Scalar(0,255,0),3);
        cv::line(img2draw,cv::Point2f(feature_points[2].x,feature_points[2].y),cv::Point2f(feature_points[3].x,feature_points[3].y),cv::Scalar(0,255,0),3);
        cv::line(img2draw,cv::Point2f(feature_points[3].x,feature_points[3].y),cv::Point2f(feature_points[0].x,feature_points[0].y),cv::Scalar(0,255,0),3);

    }
    bool getFeaturePoint(Grasp_Data grasp_point_filtered, cv::Mat color_image, cv::Mat depth_image, float font_depth)
    {
        T<<-grasp_point_filtered.x,-grasp_point_filtered.y;
//        R<<cos(grasp_point_filtered.theta - CV_PI/2),sin(grasp_point_filtered.theta - CV_PI/2),
//                -sin(grasp_point_filtered.theta - CV_PI/2),cos(grasp_point_filtered.theta - CV_PI/2);
        R<<1,0,
           0,1;
        //设置4个
        float range = 50*(0.25/font_depth);
//        float range = 50;
        detect_feature_points[0]<<- range,- range;
        detect_feature_points[1]<<+ range,- range;
        detect_feature_points[2]<<+ range,+ range;
        detect_feature_points[3]<<- range,+ range;
//         cv::blur(depth_image,depth_image,cv::Size(5,5));
        cv::inpaint( depth_image, depth_image == 0, depth_image, 10,cv::INPAINT_NS);
        for(int i = 0;i < 4;i++){
            target_feature_points[i] = R * detect_feature_points[i] - T;
            feature_points[i].x = target_feature_points[i][0];
            feature_points[i].y = target_feature_points[i][1];
            feature_points[i].z = float(depth_image.at<cv::uint16_t>(ceil(feature_points[i].y),ceil(feature_points[i].x)))*0.001;
//            cout<<"Z"<<feature_points[i].z<<endl;
//            feature_points[i].z = 0.5;
            if(feature_points[i].z == 0)
            {
                cout<<"There is a z = 0!"<<endl;
                return false;
            }
        }
        this->font_depth = font_depth;
        drawFeaturePoint(color_image);
//        cv::waitKey(1);
        return true;

    }



    void drawFeaturePoint(cv::Mat& img2draw)
    {
        drawSquares(img2draw);
    }
    std_msgs::msg::String getSpeedCommand()
    {
        auto message = std_msgs::msg::String();
        vpServo task; // Visual servoing task
        float x,y,z;float range = 50;float bias = 0;
        float xd,yd,zd;
        for(int i = 0;i < sd.size();i++){

            x = feature_points[i].x;y = feature_points[i].y;z = feature_points[i].z;
            project2CameraPlane(x,y,z);
            s[i].buildFrom(x, y, z);
            z = z - this->font_depth + 0.25;
            switch(i){
                case 0:
                    x = 320 - range;y = 240 - range + bias;project2CameraPlane(x,y,z);
                    break;
                case 1:
                    x = 320 + range;y = 240 - range + bias;project2CameraPlane(x,y,z);
                    break;
                case 2:
                    x = 320 + range;y = 240 + range + bias;project2CameraPlane(x,y,z);
                    break;
                case 3:
                    x = 320 - range;y = 240 + range + bias;project2CameraPlane(x,y,z);
                    break;
            }

            sd[i].buildFrom(x, y, z);
            task.addFeature(s[i], sd[i]);
//            cout<<"sx:"<<s[i].get_x()<<endl;
//            cout<<"sdx:"<<sd[i].get_x()<<endl;
        }
        e = task.computeError();
        vpMatrix L = task.computeInteractionMatrix();

        vpMatrix L_inverse = 1.5 * L.AtA().inverseByLU() * L.transpose();
//        cout<<"L_inverse:"<<L_inverse<<endl;
        v =  - L_inverse * e;
        v[0] *= -200;
//        v[1] = (v[1] - 0.4 * v[3]) * 20;
        v[1] *= -200;
        v[2] *= 100;
//        v[2] = 1;
//        v[5] *= 180 / 3.1415926 * 0.5;
v[5] = 0;
//        v[5] = 0;
//        for(int i = 0;i < 6;i++){
//            if(v[i] > 5)
//                v[i] = 5;
//            else if(v[i] < -5)
//                v[i] = -5;
//        }
        v = dataSmooth(v, 10, 0.05);
        message.data = "["+ to_string(v[0]) + ',' + to_string(v[1]) + ','+ to_string(v[2]) + ','+ to_string(v[5]) + ']';;


//        message.data = "["+ to_string(v) + ',' + to_string(v+1) + ','+ to_string(v+2) + ','+ to_string(v+3) + ']';



        return message;
    }
    float getError(){
        return e.sumSquare();
    }
private:

    void project2CameraPlane(float& x,float & y,float z)
    { //focal is like (x - 611.075)/610.372*1.88
        x = z * (x - 322.86)/610.372;
        y = z * (y - 238.664)/611.075;
    }
    vpColVector dataSmooth(vpColVector input, int average_num, float learning_rate){
        static int init_num = 0;
        static vpColVector v_filtered(6,0);
        if(init_num < average_num)
        {
            v_filtered += input/float(average_num);
            init_num++;
            return input;
        }
        else
        {
            v_filtered = (1 - learning_rate) * v_filtered + learning_rate * input;
            return v_filtered;
        }

    }
//    特征处理
    vector<cv::Point3f> feature_points;
//    目标特征点获取
    float average_feature_depth;
    Eigen::VectorXd T;
    Eigen::MatrixXd R;
    vector<Eigen::VectorXd> detect_feature_points;
    vector<Eigen::VectorXd> target_feature_points;
    float target_feature_depth;
//    视觉伺服
    float font_depth;
    vpColVector e;
    std::vector<vpFeaturePoint> sd; //The desired point feature.
    std::vector<vpFeaturePoint> s;
    vpColVector v;

//  友元类
    friend class GGCNNBasedVisualServo;
};
class NetGraspFeature2D{
public:
    NetGraspFeature2D(){
        this->feature_points_num = 21;
        this->range_max = 200;
        this->target_depth = 0.25;
        feature_points.resize(feature_points_num);
        T.resize(2,feature_points_num);
        R.resize(2,2);

        sd.resize(feature_points_num);s.resize(feature_points_num);
        featureModelInit(range_max);
    }
    ~NetGraspFeature2D(){}
    void drawSquares(cv::Mat& img2draw)
    {
        for( size_t i = 0; i < feature_points_num; i++ )
        {

            circle(img2draw,cv::Point2f(feature_points[i].x,feature_points[i].y),5,cv::Scalar(0,0,255),3);
            cv::putText(img2draw,to_string(i),cv::Point2f(feature_points[i].x,feature_points[i].y),cv::FONT_HERSHEY_PLAIN,2,cv::Scalar(255,0,0),1);
        }
        cv::line(img2draw,cv::Point2f(feature_points[6].x,feature_points[6].y),cv::Point2f(feature_points[8].x,feature_points[8].y),cv::Scalar(0,255,0),3);
        cv::line(img2draw,cv::Point2f(feature_points[8].x,feature_points[8].y),cv::Point2f(feature_points[10].x,feature_points[10].y),cv::Scalar(0,255,0),3);
        cv::line(img2draw,cv::Point2f(feature_points[10].x,feature_points[10].y),cv::Point2f(feature_points[12].x,feature_points[12].y),cv::Scalar(0,255,0),3);
        cv::line(img2draw,cv::Point2f(feature_points[12].x,feature_points[12].y),cv::Point2f(feature_points[6].x,feature_points[6].y),cv::Scalar(0,255,0),3);

    }
    bool getFeaturePoint(Grasp_Data grasp_point_filtered, cv::Mat color_image, cv::Mat depth_image, float font_depth)
    {
        cv::inpaint( depth_image, depth_image == 0, depth_image, 5,cv::INPAINT_NS);
        for(int i = 0; i<feature_points_num;i++){
            T(0,i) = -grasp_point_filtered.x;
            T(1,i) = -grasp_point_filtered.y;
        }
        R<<cos(this->feature_angle),sin(this->feature_angle),
                -sin(this->feature_angle),cos(this->feature_angle);
//        R<<1,0,
//                0,1;
//        R<<cos(CV_PI/6),sin(CV_PI/6),
//        -sin(CV_PI/6),cos(CV_PI/6);
        //放缩倍率
        float factor = target_depth/font_depth;
        detect_feature_points = factor * R * model_feature_points - T;

        for(int i = 0;i < feature_points_num;i++){
            feature_points[i].x = detect_feature_points(0,i);
            feature_points[i].y = detect_feature_points(1,i);
            feature_points[i].z = float(depth_image.at<cv::uint16_t>(ceil(feature_points[i].y),ceil(feature_points[i].x)))*0.001;
            if(feature_points[i].z == 0)
            {
                cout<<"There is a z = 0!"<<endl;
                return false;
            }

        }

        this->font_depth = fontDepthFilter(font_depth);
        drawFeaturePoint(color_image);
        return true;

    }



    void drawFeaturePoint(cv::Mat& img2draw)
    {
        drawSquares(img2draw);
    }
    std_msgs::msg::String getSpeedCommand()
    {
        auto message = std_msgs::msg::String();
        vpServo task; // Visual servoing task
        float x,y,z;float bias = 0;
        float xd,yd,zd;
        for(int i = 0;i < sd.size();i++){

            x = feature_points[i].x;y = feature_points[i].y;z = feature_points[i].z;
            project2CameraPlane(x,y,z);
            s[i].buildFrom(x, y, z);
            z = z - this->font_depth + target_depth;
            x = model_feature_points(0, i)+320;y = model_feature_points(1, i)+240;

            project2CameraPlane(x,y,z);
            sd[i].buildFrom(x, y, z);
            task.addFeature(s[i], sd[i]);
//            cout<<"sx:"<<s[i].get_x()<<endl;
//            cout<<"sdx:"<<sd[i].get_x()<<endl;
        }
        e = task.computeError();
        vpMatrix L = task.computeInteractionMatrix();

        vpMatrix L_inverse = 1.5 * L.AtA().inverseByLU() * L.transpose();
//        cout<<"L_inverse:"<<L_inverse<<endl;
        v =  - L_inverse * e;
        v[0] *= -200;

        v[1] *= -100;
        v[2] *= 100;

        v[5] *= 180 / 3.1415926 * 0.3;
//                v[0] *= 0;
//
//        v[1] *= 0;
//        v[2] *= 0;
//
//        v[5] *= 0;

//        for(int i = 0;i < 6;i++){
//            if(v[i] > 5)
//                v[i] = 5;
//            else if(v[i] < -5)
//                v[i] = -5;
//        }
        v = dataSmooth(v, 10, 0.05);
        message.data = "["+ to_string(v[0]) + ',' + to_string(v[1]) + ','+ to_string(v[2]) + ','+ to_string(v[5]) + ']';;


//        message.data = "["+ to_string(v) + ',' + to_string(v+1) + ','+ to_string(v+2) + ','+ to_string(v+3) + ']';



        return message;
    }
    float getError(){
        return e.sumSquare();
    }
    void setFeatureAngle(float feature_angle){
        if(feature_angle > 0)
        {
            this->feature_angle = this->target_angle - feature_angle/180*CV_PI - CV_PI;
        }
        if(feature_angle < 0)
        {
            this->feature_angle = this->target_angle - feature_angle/180*CV_PI + CV_PI;
        }
    }
    void setTargetAngle(float target_angle){

        if(target_angle > 0)
        {
            this->target_angle = target_angle;
        }
        if(target_angle < 0)
        {
            this->target_angle = target_angle;
        }
    }
private:
    void featureModelInit(float range_max){
        model_feature_points.resize(2, feature_points_num);
        detect_feature_points.resize(2, feature_points_num);
        model_feature_points(0,0) = 0;model_feature_points(1,0) = 0;
        float range_split_1_2 = range_max / 2;
        float range_split_1_4 = range_max / 4;
        model_feature_points(0,1) = range_max;model_feature_points(1,1) = 0;
        model_feature_points(0,2) = 0;model_feature_points(1,2) = -range_max;
        model_feature_points(0,3) = -range_max;model_feature_points(1,3) = 0;
        model_feature_points(0,4) = 0;model_feature_points(1,4) = range_max;

        model_feature_points(0,5) = range_split_1_2;model_feature_points(1,5) = 0;
        model_feature_points(0,6) = range_split_1_2;model_feature_points(1,6) = -range_split_1_2;
        model_feature_points(0,7) = 0;model_feature_points(1,7) = -range_split_1_2;
        model_feature_points(0,8) = -range_split_1_2;model_feature_points(1,8) = -range_split_1_2;
        model_feature_points(0,9) = -range_split_1_2;model_feature_points(1,9) = 0;
        model_feature_points(0,10) = -range_split_1_2;model_feature_points(1,10) = range_split_1_2;
        model_feature_points(0,11) = 0;model_feature_points(1,11) = range_split_1_2;
        model_feature_points(0,12) = range_split_1_2;model_feature_points(1,12) = range_split_1_2;

        model_feature_points(0,13) = range_split_1_4;model_feature_points(1,13) = 0;
        model_feature_points(0,14) = range_split_1_4;model_feature_points(1,14) = -range_split_1_4;
        model_feature_points(0,15) = 0;model_feature_points(1,15) = -range_split_1_4;
        model_feature_points(0,16) = -range_split_1_4;model_feature_points(1,16) = -range_split_1_4;
        model_feature_points(0,17) = -range_split_1_4;model_feature_points(1,17) = 0;
        model_feature_points(0,18) = -range_split_1_4;model_feature_points(1,18) = range_split_1_4;
        model_feature_points(0,19) = 0;model_feature_points(1,19) = range_split_1_4;
        model_feature_points(0,20) = range_split_1_4;model_feature_points(1,20) = range_split_1_4;


    }

    void project2CameraPlane(float& x,float & y,float z)
    { //focal is like (x - 611.075)/610.372*1.88
        x = z * (x - 322.86)/610.372;
        y = z * (y - 238.664)/611.075;
    }
    vpColVector dataSmooth(vpColVector input, int average_num, float learning_rate){
        static int init_num = 0;
        static vpColVector v_filtered(6,0);
        if(init_num < average_num)
        {
            v_filtered += input/float(average_num);
            init_num++;
            return input;
        }
        else
        {
            v_filtered = (1 - learning_rate) * v_filtered + learning_rate * input;
            return v_filtered;
        }

    }
    float fontDepthFilter(float font_depth){
        static bool if_init = false;
        static float average = 0;
        float learning_rate = 0.1;
        if(!if_init){
            average = font_depth;
            if_init = true;
            cout<<"average:"<<average<<endl;
            return average;
        }

//            float error_related = fabs(font_depth - last_font_depth)/last_font_depth;
//            cout<<"error_related:"<<error_related<<endl;
//            if(error_related > 0.1)
//            {
//                return last_font_depth;
//            }
//            else
//            {
//                last_font_depth = font_depth;
//                return font_depth;
//            }

        float error_related = fabs(font_depth - average)/average;
        cout<<"error_related:"<<error_related<<endl;
        average = (1 - learning_rate) * average + learning_rate * font_depth;
        return average;
//        if(error_related > 0.1)
//        {
//            return average;
//        }
//        else
//        {
//            return font_depth;
//        }
    }
//    特征处理
    int feature_points_num;
    float range_max;
    float target_depth;
    float target_angle;
    float feature_angle;
    vector<cv::Point3f> feature_points;
//    目标特征点获取
    float average_feature_depth;
    Eigen::MatrixXd T;
    Eigen::MatrixXd R;

    Eigen::MatrixXd model_feature_points;
    Eigen::MatrixXd detect_feature_points;
    float target_feature_depth;
//    视觉伺服
    float font_depth;
    vpColVector e;
    std::vector<vpFeaturePoint> sd; //The desired point feature.
    std::vector<vpFeaturePoint> s;
    vpColVector v;

//  友元类
    friend class GGCNNBasedVisualServo;
};
//
class GGCNNBasedVisualServo : public rclcpp::Node
{
public:
    enum OrbMatchMethod
    {
        NORMAL_METHOD,
        KNN_METHOD,
        RANSAC_METHOD
    };

    GGCNNBasedVisualServo(int frame_ignore_num)
            : Node("GGCNNBasedVisualServoNode")
    {
        this->frame_ignore_num = frame_ignore_num;
        this->subscription_align = this->create_subscription<sensor_msgs::msg::Image>(
                "/camera/aligned_depth_to_color/image_raw", 5, std::bind(&GGCNNBasedVisualServo::aligndepth_callback, this, std::placeholders::_1));
        this->subscription_color = this->create_subscription<sensor_msgs::msg::Image>(
                "/camera/color/image_raw", 5, std::bind(&GGCNNBasedVisualServo::color_callback, this, std::placeholders::_1));
        this->subscription_grasp = this->create_subscription<std_msgs::msg::String>(
                "GGCNNOutput", 5, std::bind(&GGCNNBasedVisualServo::grasp_callback, this, std::placeholders::_1));
        this->subscription_encoderdepth = this->create_subscription<std_msgs::msg::String>(
                "EncoderDepth", 5, std::bind(&GGCNNBasedVisualServo::encoderdepth_callback, this, std::placeholders::_1));
        this->publisher_speedcommand = this->create_publisher<std_msgs::msg::String>("SpeedCommand", 5);
        this->encoder_depth = 0;this->target_depth = 0;
    }

    cv::Mat getColorImage()
    {
        return this->color_image;
    }
    cv::Mat getDepthImage()
    {
        return this->depth_image;
    }
//深度>30cm 滤波抓取点，更新目标点，追踪模式，相机获取深度。
//深度>20cm 追踪模式，相机获取深度。
//深度<20cm 追踪模式，编码器获取深度。
//总偏差小于一定值且深度在25cm以内时进入抓取模式。
    void run(){
//        this->running_state
        //更新抓取点位置
        if(this->running_state.if_refresh_target_point == true){
            grasp_data_handle_tool.handleGraspData(0.2);
        }
        //更新特征位置
        switch(this->running_state.running_mode)
        {
            case RunningState::RunningMode::Tracking:
                if(! square_feature.getFeaturePoint(this->color_image))
                    return;

                break;
            default:

                break;
        }
        //更新特征深度和目标特征点
        switch(this->running_state.depth_get_mode)
        {
            case RunningState::DepthGetMode::Camera:
                if(! square_feature.refreshFeaturePoint(this->depth_image, this->encoder_depth))
                    return;
                this->target_depth = square_feature.getTargetDepth();
                encoder_depth_handle_tool.setDeltaDepth(this->encoder_depth,this->target_depth);
                if(this->running_state.if_refresh_target_point == true){
                square_feature.regreshTargetFeaturePoint(grasp_data_handle_tool.grasp_data_filtered);}
                square_feature.drawTargetFeaturePoint(this->color_image);
                break;
            case RunningState::DepthGetMode::Encoding:
                if(! square_feature.refreshFeaturePoint(this->encoder_depth))
                    return;
                this->target_depth = encoder_depth_handle_tool.getDepth(this->encoder_depth);
                if(this->running_state.if_refresh_target_point == true){
                square_feature.regreshTargetFeaturePoint(grasp_data_handle_tool.grasp_data_filtered);}
                square_feature.drawTargetFeaturePoint(this->color_image);
                break;
        }
        //根据模式选择视觉伺服、开环抓取
        switch(this->running_state.running_mode)
        {
            case RunningState::RunningMode::Tracking:
                this->publisher_speedcommand->publish(square_feature.getSpeedCommand());
                break;
            case RunningState::RunningMode::Grasp:

                break;
            case RunningState::RunningMode::Stop:

                break;
        }//f
        //更新运行状态
        this->running_state.setMode(this->target_depth, 0.2);


//        cout<<"encoder_depth:"<<this->encoder_depth<<"depth:"<<this->encoder_depth_handle_tool.getDepth(this->encoder_depth)<<endl;
    }
//深度>20cm 滤波抓取点,获取特征，更新目标特征深度（先固定0.25），相机获取深度
//深度=20cm 停止（或者特征误差小于一定值）
    void run_plus(){
//        this->running_state
        static int ignore_num = 0;
        //更新抓取点位置
        if(this->running_state.if_refresh_target_point == true){
            grasp_data_handle_tool.handleGraspData(0.2 - 0.19*(0.25/grasp_data_handle_tool.getFontDepth()));
        }
        //更新特征
        switch(this->running_state.running_mode)
        {
            case RunningState::RunningMode::Tracking:
                if(! grasp_feature.getFeaturePoint(grasp_data_handle_tool.grasp_data_filtered, this->color_image,this->depth_image,grasp_data_handle_tool.getFontDepth()))
                    return;

                break;
            default:

                break;
        }

        //根据模式选择视觉伺服、抓取、停止
        if(ignore_num < 80)
        {
            ignore_num++;
            grasp_feature.getSpeedCommand();
        }
        else {
            switch (this->running_state.running_mode) {
                case RunningState::RunningMode::Tracking:
                    this->publisher_speedcommand->publish(grasp_feature.getSpeedCommand());
                    break;
                case RunningState::RunningMode::Grasp:

                    break;
                case RunningState::RunningMode::Stop:
                    auto message = std_msgs::msg::String();
                    message.data = "[0,0,0,0]";
                    this->publisher_speedcommand->publish(message);
                    break;
            }//f
        }
        //更新运行状态
        this->running_state.setModePlus(grasp_data_handle_tool.getFontDepth(), grasp_feature.getError());


//        cout<<"encoder_depth:"<<this->encoder_depth<<"depth:"<<this->encoder_depth_handle_tool.getDepth(this->encoder_depth)<<endl;
    }
    void run_net_plus(){
        //        this->running_state
        static int ignore_num = 0;
        //更新抓取点位置
        if(this->running_state.if_refresh_target_point == true){
            if(ignore_num == 10)
            {
                net_grasp_feature.setTargetAngle(grasp_data_handle_tool.grasp_data.theta);
            }
            net_grasp_feature.setFeatureAngle(this->encoder_angle);
            grasp_data_handle_tool.handleGraspData(1);

        }
        //更新特征
        switch(this->running_state.running_mode)
        {
            case RunningState::RunningMode::Tracking:
                if(! net_grasp_feature.getFeaturePoint(grasp_data_handle_tool.grasp_data_filtered, this->color_image,this->depth_image,grasp_data_handle_tool.getFontDepth()))
                    return;

                break;
            default:

                break;
        }

        //根据模式选择视觉伺服、抓取、停止
        if(ignore_num < 80)
        {
            ignore_num++;
            net_grasp_feature.getSpeedCommand();
        }
        else {
            switch (this->running_state.running_mode) {
                case RunningState::RunningMode::Tracking:
                    this->publisher_speedcommand->publish(net_grasp_feature.getSpeedCommand());
                    break;
                case RunningState::RunningMode::Grasp:

                    break;
                case RunningState::RunningMode::Stop:
                    auto message = std_msgs::msg::String();
                    message.data = "[0,0,0,0]";
                    this->publisher_speedcommand->publish(message);
                    break;
            }//f
        }
        //更新运行状态
        this->running_state.setModePlus(grasp_data_handle_tool.getFontDepth(), net_grasp_feature.getError());


//        cout<<"encoder_depth:"<<this->encoder_depth<<"depth:"<<this->encoder_depth_handle_tool.getDepth(this->encoder_depth)<<endl;
    }
    void ORBFeature2DRefresh(OrbMatchMethod method, bool if_draw_keypoints)
    {
        static bool if_first_frame = true;
        static int frame_num = 0;
        //忽略前面几帧
        if(frame_num < this->frame_ignore_num)
        {
            frame_num ++;
            return;
        }
        //跟据不同的方法保存、匹配特征
        vector<cv::DMatch> good_matches;
        switch(method)
        {
            case NORMAL_METHOD:
                this->orb->detect(this->color_image,this->keypoints1);
                this->orb->compute(this->color_image,this->keypoints1,this->descriptors1);
                if(if_first_frame == true)
                {
                    if_first_frame = false;
                    this->bfMatcher.create(cv::NORM_HAMMING);
                    this->keypoints2.assign(this->keypoints1.begin(), this->keypoints1.end());
                    this->descriptors2 = this->descriptors1.clone();
                }
                else
                {
                    this->bfMatcher.match(this->descriptors1, this->descriptors2, this->matches);
                    double min_dist = 1000, max_dist = 0;
                    // 找出所有匹配之间的最大值和最小值
//                    for (int i = 0; i < this->descriptors1.rows; i++)
//                    {
//                        double dist = this->matches[i].distance;
//                        if (dist < min_dist) min_dist = dist;
//                        if (dist > max_dist) max_dist = dist;
//                    }
                    sort(this->matches.begin(),this->matches.end(),CompareMatch);
                    min_dist = this->matches[0].distance;
                    // 当描述子之间的匹配大于2倍的最小距离时，即认为该匹配是一个错误的匹配。
                    // 但有时描述子之间的最小距离非常小，可以设置一个经验值作为下限
                    this->keypoints2.clear();
                    for (int i = 0; i < this->descriptors1.rows; i++)
                    {
                        if (this->keypoints1[matches[i].queryIdx].pt.y < 300  && this->keypoints1[matches[i].queryIdx].pt.x < 500  && this->keypoints1[matches[i].queryIdx].pt.x > 200)
                        {
                            if(this->matches[i].distance <= max(2*min_dist, 50.0)){
                                this->keypoints2.push_back(this->keypoints1[matches[i].queryIdx]);
                                good_matches.push_back(this->matches[i]);
                            }
                            else if(this->matches[i].distance > max(2*min_dist, 30.0) && this->keypoints2.size() < 20)
                            {
                                this->keypoints2.push_back(this->keypoints1[matches[i].queryIdx]);
                                good_matches.push_back(this->matches[i]);
                            }
                        }
                    }
                    this->orb->compute(this->color_image,this->keypoints2,this->descriptors2);
                }

                break;
            default:
                cout<<"Please input correct method!"<<endl;
        }

        //绘制匹配出的特征点
        cv::Mat test;
        if(if_draw_keypoints) {
            this->color_image2draw = this->color_image.clone();
            cv::Point2i point;
            cout<<"match_size:"<<good_matches.size()<<endl;
            for (size_t i = 0; i < good_matches.size(); i++) {
                point.x = ceil(this->keypoints1[good_matches[i].queryIdx].pt.x);
                point.y = ceil(this->keypoints1[good_matches[i].queryIdx].pt.y);
                cv::putText(this->color_image2draw, std::to_string(i), point, cv::FONT_HERSHEY_PLAIN, 1,
                            cv::Scalar(255, 0, 0), 2);
            }
            cv::imshow("Feature2D1",this->color_image2draw);

            test = this->color_image.clone();
            for (size_t i = 0; i < good_matches.size(); i++) {
                point.x = ceil(this->keypoints2[good_matches[i].trainIdx].pt.x);
                point.y = ceil(this->keypoints2[good_matches[i].trainIdx].pt.y);
                cv::putText(test, std::to_string(i), point, cv::FONT_HERSHEY_PLAIN, 1,
                            cv::Scalar(255, 0, 0), 2);
            }
            cv::imshow("Feature2D2",test);
            cv::waitKey(1);
        }

//        this->keypoints2.assign(this->keypoints1.begin(), this->keypoints1.end());
//        this->descriptors2 = this->descriptors1.clone();

    }
    void drawGraspData(){
        this->color_image2draw = this->color_image.clone();
        float x = grasp_data_handle_tool.grasp_data_filtered.x;float y = grasp_data_handle_tool.grasp_data_filtered.y;
        float angle = grasp_data_handle_tool.grasp_data_filtered.theta;float width = grasp_data_handle_tool.grasp_data_filtered.width;
        cv::circle(this->color_image2draw,cv::Point((int)x, (int)y), 5, cv::Scalar(255, 0, 0), 4);
        float alpha = CV_PI/2 - angle;
        vector<cv::Point2f> p(4) ;
        p[0] = (cv::Point2f(ceil(x+width*cos(angle)-0.5*width*cos(alpha)), ceil(y-width*sin(angle)-0.5*width*sin(alpha))));
        p[1] = (cv::Point2f(ceil(x+width*cos(angle)+0.5*width*cos(alpha)), ceil(y-width*sin(angle)+0.5*width*sin(alpha))));
        p[2] = (cv::Point2f(ceil(x-width*cos(angle)+0.5*width*cos(alpha)), ceil(y+width*sin(angle)+0.5*width*sin(alpha))));
        p[3] = (cv::Point2f(ceil(x-width*cos(angle)-0.5*width*cos(alpha)), ceil(y+width*sin(angle)-0.5*width*sin(alpha))));
        for(size_t i = 0;i < 3;i++)
        {
            cv::line(this->color_image2draw, p[i], p[i+1], cv::Scalar(0, 255, 0), 2);
        }
        cv::line(this->color_image2draw, p[3], p[0], cv::Scalar(0, 255, 0), 2);
        cv::imshow("grasp_filtered", this->color_image2draw);

//        this->color_image2draw2 = this->color_image.clone();
//        x = grasp_data_handle_tool.grasp_data.x;y = grasp_data_handle_tool.grasp_data.y;
//        angle = grasp_data_handle_tool.grasp_data.theta; width = grasp_data_handle_tool.grasp_data.width;
//        cv::circle(this->color_image2draw2,cv::Point((int)x, (int)y), 5, cv::Scalar(255, 0, 0), 4);
//        alpha = CV_PI/2 - angle;
//        p[0] = (cv::Point2f(ceil(x+width*cos(angle)-0.5*width*cos(alpha)), ceil(y-width*sin(angle)-0.5*width*sin(alpha))));
//        p[1] = (cv::Point2f(ceil(x+width*cos(angle)+0.5*width*cos(alpha)), ceil(y-width*sin(angle)+0.5*width*sin(alpha))));
//        p[2] = (cv::Point2f(ceil(x-width*cos(angle)+0.5*width*cos(alpha)), ceil(y+width*sin(angle)+0.5*width*sin(alpha))));
//        p[3] = (cv::Point2f(ceil(x-width*cos(angle)-0.5*width*cos(alpha)), ceil(y+width*sin(angle)-0.5*width*sin(alpha))));
//        for(size_t i = 0;i < 3;i++)
//        {
//            cv::line(this->color_image2draw2, p[i], p[i+1], cv::Scalar(0, 255, 0), 2);
//        }
//        cv::line(this->color_image2draw2, p[3], p[0], cv::Scalar(0, 255, 0), 2);
//        cv::imshow("grasp", this->color_image2draw2);
    }

private:
//图片
    cv::Mat color_image;
    cv::Mat depth_image;
    cv::Mat color_image2draw;
    cv::Mat color_image2draw2;
//orb特征检测、匹配
    cv::Ptr<cv::ORB> orb = cv::ORB::create();
    vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;
    vector<cv::DMatch> matches;
    cv::BFMatcher bfMatcher;
    int frame_ignore_num;
//抓取点数据处理
    GraspDataHandle grasp_data_handle_tool;
    float target_depth;
//二维码法视觉伺服
    SquareFeature2D square_feature;
    GraspFeature2D grasp_feature;
    NetGraspFeature2D net_grasp_feature;
//机械臂端
    float encoder_depth;
    float encoder_angle;
    EncoderDepthHandle encoder_depth_handle_tool;
//运行
    RunningState running_state;

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_color;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_align;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_grasp;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_encoderdepth;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_speedcommand;
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
    void grasp_callback(const std_msgs::msg::String::SharedPtr msg){
        RCLCPP_INFO(this->get_logger(), "I heard GRASP: %s",msg->data.c_str());
        this->grasp_data_handle_tool.setGraspData(msg->data.c_str());
        visualready.setGraspOK(true);
//        grasp_data_handle_tool.showGraspData();
    }
    void encoderdepth_callback(const std_msgs::msg::String::SharedPtr msg){
        RCLCPP_INFO(this->get_logger(), "I heard EncoderDepth: %s",msg->data.c_str());
        this->encoder_depth = std::atof(msg->data.c_str());
        this->encoder_angle = std::atof(msg->data.c_str());
    }
//友元类
    friend class SquareFeature2D;
};

int main(int argc, char * argv[])
{
//ROS
    rclcpp::init(argc, argv);
    std::shared_ptr<GGCNNBasedVisualServo> VisualServoTool =
            std::make_shared<GGCNNBasedVisualServo> (10);
//opencv
//    std::string OPENCV_COLOR_WINDOW = "color_image";
//    std::string OPENCV_DEPTH_WINDOW = "depth_image";
//    cv::namedWindow(OPENCV_COLOR_WINDOW);
//    cv::namedWindow(OPENCV_DEPTH_WINDOW);

    while(rclcpp::ok()){
        rclcpp::spin_some(VisualServoTool);
        if(visualready.getVSOK() == true)
        {

            visualready.refreshOK();
            if(!VisualServoTool->getColorImage().empty()) {
                VisualServoTool->run_net_plus();
//                VisualServoTool->ORBFeature2DRefresh(GGCNNBasedVisualServo::NORMAL_METHOD,true);
                VisualServoTool->drawGraspData();
                cv::waitKey(1);
            }
        }
    }

    rclcpp::shutdown();
    return 0;
}


//class NetGraspFeature2D{
//public:
//    NetGraspFeature2D(){
//        this->split_size = 4;
//        this->range_max = 200;
//        feature_points.resize(split_size * split_size);
//        T.resize(2);
//        R.resize(2,2);
//        target_feature_points.resize(split_size * split_size);
//        detect_feature_points.resize(split_size * split_size);
//        for(size_t i = 0;i < split_size*split_size;i++){
//            target_feature_points[i].resize(2);
//            detect_feature_points[i].resize(2);
//        }
//        sd.resize(split_size * split_size);s.resize(split_size * split_size);
//
//    }
//    ~NetGraspFeature2D(){}
//    void drawSquares(cv::Mat& img2draw)
//    {
//        for( size_t i = 0; i < split_size * split_size; i++ )
//        {
//
//            circle(img2draw,cv::Point2f(feature_points[i].x,feature_points[i].y),10,cv::Scalar(0,0,255),3);
////            circle(img2draw,cv::Point2f(sd[i].get_x(),sd[i].get_y()),10,cv::Scalar(0,255,0),3);
////            float x = sd[i].get_x();float y = sd[i].get_y();
////            cv::putText(img2draw,to_string(i),cv::Point2f(x,y),cv::FONT_HERSHEY_PLAIN,2,cv::Scalar(255,0,0),2);
////            project2CameraPlane(x,y,0.25);
////            sd[i].set_xyZ(x,y,0.25);
//            cv::putText(img2draw,to_string(i),cv::Point2f(feature_points[i].x,feature_points[i].y),cv::FONT_HERSHEY_PLAIN,2,cv::Scalar(255,0,0),2);
//
//        }
//        cv::line(img2draw,cv::Point2f(feature_points[0].x,feature_points[0].y),cv::Point2f(feature_points[split_size-1].x,feature_points[split_size-1].y),cv::Scalar(0,255,0),3);
//        cv::line(img2draw,cv::Point2f(feature_points[split_size-1].x,feature_points[split_size-1].y),cv::Point2f(feature_points[split_size*split_size-1].x,feature_points[split_size*split_size-1].y),cv::Scalar(0,255,0),3);
//        cv::line(img2draw,cv::Point2f(feature_points[split_size*split_size-1].x,feature_points[split_size*split_size-1].y),cv::Point2f(feature_points[split_size*(split_size-1)].x,feature_points[split_size*(split_size-1)].y),cv::Scalar(0,255,0),3);
//        cv::line(img2draw,cv::Point2f(feature_points[split_size*(split_size-1)].x,feature_points[split_size*(split_size-1)].y),cv::Point2f(feature_points[0].x,feature_points[0].y),cv::Scalar(0,255,0),3);
//
//    }
//    bool getFeaturePoint(Grasp_Data grasp_point_filtered, cv::Mat color_image, cv::Mat depth_image, float font_depth)
//    {
//        cv::inpaint( depth_image, depth_image == 0, depth_image, 5,cv::INPAINT_NS);
//        T<<-grasp_point_filtered.x,-grasp_point_filtered.y;
////        R<<cos(grasp_point_filtered.theta - CV_PI/2),sin(grasp_point_filtered.theta - CV_PI/2),
////                -sin(grasp_point_filtered.theta - CV_PI/2),cos(grasp_point_filtered.theta - CV_PI/2);
//        R<<1,0,
//                0,1;
//        //设置4个
//        float range = this->range_max*(0.25/font_depth);
//        float split_range = range / (split_size-1);
//        float left =  - 0.5 * range;
//        float top  =  - 0.5 * range;
////        float range = 50;
//        for(int i = 0;i < split_size;i++){
//
//            for(int j = 0;j < split_size;j++){
//                detect_feature_points[i * split_size + j][0] = left + j * split_range;
//                detect_feature_points[i * split_size + j][1] = top + i * split_range;
//                target_feature_points[i * split_size + j] = R * detect_feature_points[i * split_size + j] - T;
//                feature_points[i * split_size + j].x = target_feature_points[i * split_size + j][0];
//                feature_points[i * split_size + j].y = target_feature_points[i * split_size + j][1];
//                feature_points[i * split_size + j].z = float(depth_image.at<cv::uint16_t>(ceil(feature_points[i * split_size + j].y),ceil(feature_points[i * split_size + j].x)))*0.001;
//                if(feature_points[i * split_size + j].z == 0)
//                {
//                    cout<<"There is a z = 0!"<<endl;
//                    return false;
//                }
//            }
//        }
//
//        this->font_depth = font_depth;
//        drawFeaturePoint(color_image);
//        return true;
//
//    }
//
//
//
//    void drawFeaturePoint(cv::Mat& img2draw)
//    {
//        drawSquares(img2draw);
//    }
//    std_msgs::msg::String getSpeedCommand()
//    {
//        auto message = std_msgs::msg::String();
//        vpServo task; // Visual servoing task
//        float x,y,z;float bias = 0;
//        float xd,yd,zd;
//        for(int i = 0;i < sd.size();i++){
//
//            x = feature_points[i].x;y = feature_points[i].y;z = feature_points[i].z;
//            project2CameraPlane(x,y,z);
//            s[i].buildFrom(x, y, z);
//            z = z - this->font_depth + 0.25;
//            float split_range = range_max / (split_size-1);
//            float left = 320 - 0.5 * range_max;
//            float top  = 240 - 0.5 * range_max;
//
//            x = left + split_range * (i % int(split_size));
//            y = top + split_range * (i / int(split_size));
//
//            project2CameraPlane(x,y,z);
//            sd[i].buildFrom(x, y, z);
//            task.addFeature(s[i], sd[i]);
////            cout<<"sx:"<<s[i].get_x()<<endl;
////            cout<<"sdx:"<<sd[i].get_x()<<endl;
//        }
//        e = task.computeError();
//        vpMatrix L = task.computeInteractionMatrix();
//
//        vpMatrix L_inverse = 1.5 * L.AtA().inverseByLU() * L.transpose();
////        cout<<"L_inverse:"<<L_inverse<<endl;
//        v =  - L_inverse * e;
//        v[0] *= -200;
////        v[1] = (v[1] - 0.4 * v[3]) * 20;
//        v[1] *= -100;
//        v[2] *= 100;
////        v[2] = 1;
////        v[5] *= 180 / 3.1415926 * 0.5;
//        v[5] = 0;
////        v[5] = 0;
////        for(int i = 0;i < 6;i++){
////            if(v[i] > 5)
////                v[i] = 5;
////            else if(v[i] < -5)
////                v[i] = -5;
////        }
//        v = dataSmooth(v, 10, 0.05);
//        message.data = "["+ to_string(v[0]) + ',' + to_string(v[1]) + ','+ to_string(v[2]) + ','+ to_string(v[5]) + ']';;
//
//
////        message.data = "["+ to_string(v) + ',' + to_string(v+1) + ','+ to_string(v+2) + ','+ to_string(v+3) + ']';
//
//
//
//        return message;
//    }
//    float getError(){
//        return e.sumSquare();
//    }
//private:
//
//    void project2CameraPlane(float& x,float & y,float z)
//    { //focal is like (x - 611.075)/610.372*1.88
//        x = z * (x - 322.86)/610.372;
//        y = z * (y - 238.664)/611.075;
//    }
//    vpColVector dataSmooth(vpColVector input, int average_num, float learning_rate){
//        static int init_num = 0;
//        static vpColVector v_filtered(6,0);
//        if(init_num < average_num)
//        {
//            v_filtered += input/float(average_num);
//            init_num++;
//            return input;
//        }
//        else
//        {
//            v_filtered = (1 - learning_rate) * v_filtered + learning_rate * input;
//            return v_filtered;
//        }
//
//    }
////    特征处理
//    float split_size;
//    float range_max;
//    vector<cv::Point3f> feature_points;
////    目标特征点获取
//    float average_feature_depth;
//    Eigen::VectorXd T;
//    Eigen::MatrixXd R;
//    vector<Eigen::VectorXd> detect_feature_points;
//    vector<Eigen::VectorXd> target_feature_points;
//    float target_feature_depth;
////    视觉伺服
//    float font_depth;
//    vpColVector e;
//    std::vector<vpFeaturePoint> sd; //The desired point feature.
//    std::vector<vpFeaturePoint> s;
//    vpColVector v;
//
////  友元类
//    friend class GGCNNBasedVisualServo;
//};