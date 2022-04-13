//#include <memory>
//#include "rclcpp/rclcpp.hpp"
//#include "std_msgs/msg/string.hpp"
//#include <librealsense2/rs.hpp>
//#include <sensor_msgs/msg/point_cloud2.hpp>
//#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
//#include "realsense/rs_factory.hpp"
//#include <pcl_conversions/pcl_conversions.h>
//#include <pcl/visualization/pcl_visualizer.h>
//#include <limits>
//#include <fstream>
//#include <vector>
//#include <Eigen/Core>
//#include <pcl/memory.h>
//#include <pcl/pcl_macros.h>
//#include <pcl/point_types.h>
//#include <pcl/point_cloud.h>
//#include <pcl/io/pcd_io.h>
//#include <pcl/kdtree/kdtree_flann.h>
//#include <pcl/filters/passthrough.h>
//#include <pcl/filters/voxel_grid.h>
//#include <pcl/features/normal_3d.h>
//#include <pcl/features/fpfh.h>
//#include <pcl/registration/ia_ransac.h>
//#include <pcl/sample_consensus/method_types.h>
//#include <pcl/sample_consensus/model_types.h>
//#include <pcl/segmentation/sac_segmentation.h>
//#include <pcl/filters/extract_indices.h>
//#include <pcl/features/vfh.h>
//#include <pcl/point_types.h>
//#include <pcl/point_cloud.h>
//#include <pcl/console/print.h>
//#include <pcl/io/pcd_io.h>
//#include <boost/filesystem.hpp>
//#include <flann/flann.h>
//#include <flann/io/hdf5.h>
//#include <fstream>
//#include <pcl/console/parse.h>
//#include <pcl/console/print.h>
//#include <pcl/point_types.h>
//#include <pcl/point_cloud.h>
//#include <pcl/common/common.h>
//#include <pcl/common/centroid.h> // for compute3DCentroid
//#include <pcl/visualization/pcl_visualizer.h>
//#include <pcl/console/parse.h>
//#include <pcl/console/print.h>
//#include <pcl/io/pcd_io.h>
//#include <iostream>
//#include <limits>
//#include <boost/filesystem.hpp>
//#include <boost/algorithm/string/replace.hpp> // for replace_last
//#include <image_transport/image_transport.h>
//#include <cv_bridge/cv_bridge.h>
////#include <sensor_msgs/image_encodings.h>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/opencv.hpp>
//
//bool ready = false;
//class VSReady
//{
//private:
//    bool AlignOK;
//    bool ColorOK;
//    bool VSOK;
//public:
//    VSReady()
//    {
//        AlignOK = false;
//        ColorOK = false;
//        VSOK = false;
//    }
//    void setAlignOK(bool setflag)
//    {
//        this->AlignOK = setflag;
//        if(this->ColorOK == true)
//            this->VSOK = true;
//    }
//    void setColorOK(bool setflag)
//    {
//        this->ColorOK = setflag;
//        if(this->AlignOK == true)
//            this->VSOK = true;
//    }
//    bool getVSOK(void)
//    {
//        return this->VSOK;
//    }
//    void refreshOK(void)
//    {
//        AlignOK = false;
//        ColorOK = false;
//        VSOK = false;
//    }
//    ~VSReady(){}
//};
//VSReady visualready;
//class PhotoCapture : public rclcpp::Node
//{
//public:
//    enum ModeType
//    {
//        ALIGN_DEPTH_TO_COLOR,
//        COLOR,
//        POINTCLOUD2,
//        CAMERA_INFO
//    };
//    PhotoCapture()
//            : Node("PhotoCaptureNode")
//    {
//        subscription_pointcloud2 = this->create_subscription<sensor_msgs::msg::PointCloud2>(
//                "/camera/pointcloud", 1, std::bind(&PhotoCapture::pointcloud2_callback, this, std::placeholders::_1));
//    }
//    PhotoCapture(int argc, char * argv[])
//            : Node("PhotoCaptureNode")
//    {
//        if(pcl::console::find_switch (argc, argv, "-p"))
//            mode = POINTCLOUD2;
//        else if(pcl::console::find_switch (argc, argv, "-c"))
//            mode = COLOR;
//        else if(pcl::console::find_switch (argc, argv, "-a"))
//            mode = ALIGN_DEPTH_TO_COLOR;
//        else if(pcl::console::find_switch (argc, argv, "-info"))
//            mode = CAMERA_INFO;
//
//        switch(mode)
//        {
//            case POINTCLOUD2:
//                subscription_pointcloud2 = this->create_subscription<sensor_msgs::msg::PointCloud2>(
//                        "/camera/pointcloud", 1, std::bind(&PhotoCapture::pointcloud2_callback, this, std::placeholders::_1));
//                break;
//            case ALIGN_DEPTH_TO_COLOR:
//                subscription_align = this->create_subscription<sensor_msgs::msg::Image>(
//                        "/camera/aligned_depth_to_color/image_raw", 5, std::bind(&PhotoCapture::aligndepth_callback, this, std::placeholders::_1));
//                subscription_color = this->create_subscription<sensor_msgs::msg::Image>(
//                        "/camera/color/image_raw", 5, std::bind(&PhotoCapture::color_callback, this, std::placeholders::_1));
////                this->cv_ptr =  cv_bridge::CvImagePtr(new cv_bridge::CvImage);
//                break;
//            case COLOR:
//                subscription_color = this->create_subscription<sensor_msgs::msg::Image>(
//                        "/camera/color/image_raw", 5, std::bind(&PhotoCapture::color_callback, this, std::placeholders::_1));
//                break;
//            case CAMERA_INFO:
//                subscription_info = this->create_subscription<sensor_msgs::msg::CameraInfo>(
//                        "/camera/aligned_depth_to_color/camera_info", 1, std::bind(&PhotoCapture::info_callback, this, std::placeholders::_1));
//                break;
//        }
//
//    }
//    ModeType getMode()
//    {
//        return mode;
//    }
//    pcl::PCLPointCloud2 getPointCloud2()
//    {
//        return this->msgPointCloud;
//    }
//    cv::Mat getColorImage()
//    {
//        return color_image;
//    }
//    cv::Mat getAlignDepthImage()
//    {
//        return depth_image;
//    }
//private:
//
//    pcl::PCLPointCloud2 msgPointCloud;
//    cv::Mat color_image;
//    cv::Mat depth_image;
//    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr subscription_pointcloud2;
//    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_color;
//    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_align;
//    rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr subscription_info;
//    ModeType mode;
//
//    void pointcloud2_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
//    {
//        RCLCPP_INFO(this->get_logger(), "I heard PC2: %d",msg->width);
//        pcl_conversions::toPCL(*msg,this->msgPointCloud);
//        ready = true;
//    }
//    void aligndepth_callback(const sensor_msgs::msg::Image::SharedPtr msg)
//    {
//        RCLCPP_INFO(this->get_logger(), "I heard ALIGN: %d",msg->data[500]);
//        depth_image = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_16UC1)->image;
//        visualready.setAlignOK(true);
//
//    }
//    void color_callback(const sensor_msgs::msg::Image::SharedPtr msg)
//    {
//        RCLCPP_INFO(this->get_logger(), "I heard COLOR: %s",msg->encoding.c_str());
//        color_image = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;
//        visualready.setColorOK(true);
//
//    }
//    void info_callback(const sensor_msgs::msg::CameraInfo::SharedPtr msg)
//    {
//        float fx = msg->k[0] , cx = msg->k[2],fy = cx = msg->k[4],cy = msg->k[5];
//        std::cout<<"fx:"<<fx<<" fy:"<<fy<<" cx:"<<cx<<" cy:"<<cy<<std::endl;
//        ready = true;
//    }
//};
//bool ifGetPicture = false;
//void keyboardEventOccurred (const pcl::visualization::KeyboardEvent& event,
//                            void* nothing)
//{
//    if (event.getKeySym () == "space" && event.keyDown ())
//        ifGetPicture = true;
//}
//
//int main(int argc, char * argv[])
//{
//    //实时获取相机深度图对应的点云
//
//    rclcpp::init(argc, argv);
//    std::shared_ptr<PhotoCapture> PhotCaptureTool =
//                std::make_shared<PhotoCapture> (argc,argv);
//    int num = 0;
//    //parameter for pointcloud2
//    pcl::PCLPointCloud2 msgPointCloud;
//    pcl::PointCloud<pcl::PointXYZ>::Ptr msgPointCloud_pcp(new pcl::PointCloud<pcl::PointXYZ>());
//    pcl::visualization::PCLVisualizer viewer ("test demo");
//    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> cloud_tr_color_h (msgPointCloud_pcp, 200, 20, 20);
//    pcl::PCDWriter writer;
//    //parameter for colorimg
//    cv::Mat color_image;
//    cv::Mat depth_image;
//    std::string OPENCV_WINDOW = "color_image";
//    std::string OPENCV_DEPTH_WINDOW = "depth_image";
//    int key;
//    switch(PhotCaptureTool->getMode())
//    {
//        case PhotoCapture::ModeType::POINTCLOUD2:
//            // 设置背景色
//            viewer.addPointCloud<pcl::PointXYZ> (msgPointCloud_pcp,"cloud");
//            viewer.setBackgroundColor(0.0, 0.0, 0.5); // Setting background to a dark grey
//            viewer.registerKeyboardCallback (&keyboardEventOccurred, (void*) NULL);
//            // 设置渲染属性（点大小）
//            viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2);
//            viewer.addCoordinateSystem(1.0);
//            break;
//        case PhotoCapture::ModeType::ALIGN_DEPTH_TO_COLOR:
//            OPENCV_DEPTH_WINDOW = "depth_image";
//            cv::namedWindow(OPENCV_DEPTH_WINDOW);
//            OPENCV_WINDOW = "color_image";
//            cv::namedWindow(OPENCV_WINDOW);
//            break;
//        case PhotoCapture::ModeType::COLOR:
//            OPENCV_WINDOW = "color_image";
//            cv::namedWindow(OPENCV_WINDOW);
//            break;
//    }
//
//
//
//    while(rclcpp::ok()){
//
////
//        rclcpp::spin_some(PhotCaptureTool);
////        if(ready == true || visualready.getVSOK() == true)
////        {
////            ready = false;
////            visualready.refreshOK();
////            switch(PhotCaptureTool->getMode())
////            {
////                case PhotoCapture::ModeType::POINTCLOUD2:
////                    viewer.spinOnce ();
////                    msgPointCloud = PhotCaptureTool->getPointCloud2();
////                    pcl::fromPCLPointCloud2(msgPointCloud, *msgPointCloud_pcp);
////                    viewer.updatePointCloud<pcl::PointXYZ>(msgPointCloud_pcp,cloud_tr_color_h, "cloud");
////                    if(ifGetPicture)
////                    {
////                        ifGetPicture = false;
////                        std::stringstream ss;
////                        ss<<"/home/wzf/main_folder/MyProject/Graduation_project/src/realsense/data/pointcloud-img/pointcloud"<<num<<".pcd";
////                        writer.write<pcl::PointXYZ> (ss.str(), *msgPointCloud_pcp,false);
////                        num++;
////                        std::cout<<"saved successfully!"<<std::endl;
////                    }
////                    break;
////                case PhotoCapture::ModeType::ALIGN_DEPTH_TO_COLOR:
////                    color_image = PhotCaptureTool->getColorImage();
////                    depth_image = PhotCaptureTool->getAlignDepthImage();
////                    cv::imshow(OPENCV_WINDOW,color_image);
////                    cv::imshow(OPENCV_DEPTH_WINDOW,depth_image*16);
////                    key = cv::waitKey(10);
////                    if(key == 'q')
////                    {
////                        key = 0;
////                        std::stringstream ss1,ss2;
////                        ss1<<"/home/wzf/main_folder/MyProject/Graduation_project/src/realsense/data/aligned-depth-img/color"<<num<<".png";
////                        ss1<<"/home/wzf/main_folder/MyProject/Graduation_project/src/realsense/data/aligned-depth-img/depth"<<num<<".tiff";
////                        num++;
////                        cv::imwrite(ss1.str(),color_image);
////                        cv::imwrite(ss2.str(),depth_image);
////                        std::cout<<"saved successfully!"<<std::endl;
////                    }
////                    break;
////                case PhotoCapture::ModeType::COLOR:
////                    color_image = PhotCaptureTool->getColorImage();
////                    cv::imshow(OPENCV_WINDOW,color_image);
////                    key = cv::waitKey(10);
////                    if(key == 'q')
////                    {
////                        key = 0;
////                        std::stringstream ss;
////                        ss<<"/home/wzf/main_folder/MyProject/Graduation_project/src/realsense/data/color-img/color"<<num<<".png";
////                        num++;
////                        cv::imwrite(ss.str(),color_image);
////                        std::cout<<"saved successfully!"<<std::endl;
////                    }
////                    break;
////            }
////        }
////
////
//////
////
//
//
//    }
//
//
//
//
////    rclcpp::spin(std::make_shared<ObjectRecognizer>());
//    rclcpp::shutdown();
//    return 0;
//}

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

class PhotoCapture : public rclcpp::Node
{
public:
    PhotoCapture()
            : Node("PhotoCaptureNode")
    {
        this->subscription_align = this->create_subscription<sensor_msgs::msg::Image>(
                "/camera/aligned_depth_to_color/image_raw", 5, std::bind(&PhotoCapture::aligndepth_callback, this, std::placeholders::_1));
        this->subscription_color = this->create_subscription<sensor_msgs::msg::Image>(
                "/camera/color/image_raw", 5, std::bind(&PhotoCapture::color_callback, this, std::placeholders::_1));

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
    std::shared_ptr<PhotoCapture> PhotoCaptureTool =
            std::make_shared<PhotoCapture> ();
//opencv
    std::string OPENCV_COLOR_WINDOW = "color_image";
    std::string OPENCV_DEPTH_WINDOW = "depth_image";
    cv::namedWindow(OPENCV_COLOR_WINDOW);
    cv::namedWindow(OPENCV_DEPTH_WINDOW);
    char key;int num = 0;

    while(rclcpp::ok()){
        rclcpp::spin_some(PhotoCaptureTool);
        if(visualready.getVSOK() == true)
        {

            visualready.refreshOK();
            if(!PhotoCaptureTool->getColorImage().empty()) {
                cv::imshow(OPENCV_COLOR_WINDOW, PhotoCaptureTool->getColorImage());
                cv::imshow(OPENCV_DEPTH_WINDOW,PhotoCaptureTool->getDepthImage()*16);
                key = cv::waitKey(10);
                if(key == 'q')
                {
                    key = 0;
                    std::stringstream ss1,ss2;
                    ss1<<"/home/wzf/main_folder/MyProject/Graduation_project/src/realsense/data/aligned-depth-img/color"<<num<<".jpg";
                    ss2<<"/home/wzf/main_folder/MyProject/Graduation_project/src/realsense/data/aligned-depth-img/depth"<<num<<".tiff";
                    num++;
                    cv::imwrite(ss1.str(),PhotoCaptureTool->getColorImage());
                    cv::imwrite(ss2.str(),PhotoCaptureTool->getDepthImage());
                    std::cout<<"saved successfully!"<<std::endl;
                }
            }
        }
    }

    rclcpp::shutdown();
    return 0;
}