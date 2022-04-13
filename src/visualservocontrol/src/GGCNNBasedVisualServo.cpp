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
//#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"

using namespace std;

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

class GGCNNBasedVisualServo : public rclcpp::Node
{
public:
    GGCNNBasedVisualServo()
            : Node("GGCNNBasedVisualServoNode")
    {
        this->subscription_align = this->create_subscription<sensor_msgs::msg::Image>(
                "/camera/aligned_depth_to_color/image_raw", 5, std::bind(&GGCNNBasedVisualServo::aligndepth_callback, this, std::placeholders::_1));
        this->subscription_color = this->create_subscription<sensor_msgs::msg::Image>(
                "/camera/color/image_raw", 5, std::bind(&GGCNNBasedVisualServo::color_callback, this, std::placeholders::_1));

    }

    cv::Mat getColorImage()
    {
        return this->color_image;
    }

    cv::Mat getDepthImage()
    {
        return this->depth_image;
    }
private:
    cv::Mat color_image;
    cv::Mat depth_image;

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_color;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_align;


    //subscriber callback
    void aligndepth_callback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
//        RCLCPP_INFO(this->get_logger(), "I heard ALIGN: %d",msg->data[500]);
        depth_image = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_16UC1)->image;
        visualready.setAlignOK(true);
    }
    void color_callback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
//        RCLCPP_INFO(this->get_logger(), "I heard COLOR: %s",msg->encoding.c_str());
        color_image = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;
        visualready.setColorOK(true);
    }
};

int main(int argc, char * argv[])
{
//ROS
    rclcpp::init(argc, argv);
    std::shared_ptr<GGCNNBasedVisualServo> VisualServoTool =
            std::make_shared<GGCNNBasedVisualServo> ();
//opencv
    std::string OPENCV_COLOR_WINDOW = "color_image";
    std::string OPENCV_DEPTH_WINDOW = "depth_image";
    cv::namedWindow(OPENCV_COLOR_WINDOW);
    cv::namedWindow(OPENCV_DEPTH_WINDOW);
    char key;int num = 0;

    while(rclcpp::ok()){
        rclcpp::spin_some(VisualServoTool);
        if(visualready.getVSOK() == true)
        {

            visualready.refreshOK();
            if(!VisualServoTool->getColorImage().empty()) {
                cv::imshow(OPENCV_COLOR_WINDOW, VisualServoTool->getColorImage());
                cv::imshow(OPENCV_DEPTH_WINDOW,VisualServoTool->getDepthImage()*16);
                key = cv::waitKey(10);

            }
        }
    }

    rclcpp::shutdown();
    return 0;
}