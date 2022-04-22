#include <memory>
#include <string>
#include <iostream>
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

using namespace std;

//小模块类
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

    float x;
    float y;
    float theta;
    float width;
private:

} ;
VSReady visualready;
class GraspDataHandle{
public:

    GraspDataHandle(){
        this->grasp_data = Grasp_Data();
    }
    ~GraspDataHandle(){}
    void setGraspData(const string& msg){
        if(*msg.begin() == '[' && *(msg.end()-1) == ']') {
            string s1(msg.begin()+1,msg.end()-1),s2;
            stringstream ss(s1);int num = 0;
            while(getline(ss, s2, ',')) {

                this->grasp_data[num] = std::atof(s2.c_str());
                num++;
            }
        }
        else{
            cout <<"Error MSG Of Grasp!"<<endl;
            return ;
        }
    }
    void showGraspData(){
        cout<<"x:"<<grasp_data.x<<"y:"<<grasp_data.y<<"theta:"<<grasp_data.theta<<"width:"<<grasp_data.width<<endl;
    }

private:
    Grasp_Data grasp_data;
};
bool CompareMatch(cv::DMatch m1, cv::DMatch m2)
{
    return m1.distance < m2.distance;
}
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
    }

    cv::Mat getColorImage()
    {
        return this->color_image;
    }
    cv::Mat getDepthImage()
    {
        return this->depth_image;
    }

    void Feature2DRefresh(OrbMatchMethod method, bool if_draw_keypoints)
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
                            else if(this->matches[i].distance > max(2*min_dist, 30.0) && this->keypoints2.size() < 4)
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


private:
//图片
    cv::Mat color_image;
    cv::Mat depth_image;
    cv::Mat color_image2draw;
//orb特征检测、匹配
    cv::Ptr<cv::ORB> orb = cv::ORB::create();
    vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;
    vector<cv::DMatch> matches;
    cv::BFMatcher bfMatcher;
    int frame_ignore_num;
//抓取点数据处理
    GraspDataHandle grasp_data_handle_tool;



    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_color;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_align;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_grasp;







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
//        RCLCPP_INFO(this->get_logger(), "I heard GRASP: %s",msg->data.c_str());
        grasp_data_handle_tool.setGraspData(msg->data.c_str());
        grasp_data_handle_tool.showGraspData();
    }
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
                VisualServoTool->Feature2DRefresh(GGCNNBasedVisualServo::NORMAL_METHOD,false);

            }
        }
    }

    rclcpp::shutdown();
    return 0;
}