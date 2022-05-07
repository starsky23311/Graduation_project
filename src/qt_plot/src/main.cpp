#include <chrono>
#include <memory>
#include <iostream>
using namespace std;
//QT
#include "qt_plot/mainwindow.h"
#include <QApplication>
//ROS
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

//class DataReceiver : public rclcpp::Node
//{
//public:
//    DataReceiver()
//            : Node("DataReceiverNode")
//    {
//        this->subscription_speedcommand = this->create_subscription<std_msgs::msg::String>(
//                "EncoderDepth", 5, std::bind(&DataReceiver::speedcommand_callback, this, std::placeholders::_1));
//        this->v.resize(6);
//    }
//    vector<float> getSpeedCommand()
//    {
//        return this->v;
//    }
//private:
////速度指令
//    vector<float> v;
//
//    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_speedcommand;
//    void speedcommand_callback(const std_msgs::msg::String::SharedPtr msg)
//    {
//        RCLCPP_INFO(this->get_logger(), "I heard EncoderDepth: %s",msg->data.c_str());
////        this->encoder_depth = std::atof(msg->data.c_str());
//    }
//
//
//
//
//};

int main(int argc, char *argv[])
{
//    rclcpp::init(argc, argv);
//    std::shared_ptr<DataReceiver> DataReceiverTool = std::make_shared<DataReceiver>() ;

    QApplication a(argc, argv);
    MainWindow w;

    //开启QT线程。
    w.show();
//    rclcpp::spin_some(DataReceiverTool);
//    while(rclcpp::ok()){
//        rclcpp::spin_some(DataReceiverTool);
//        vector<float> v(4);
//        v.push_back(1);v.push_back(1);v.push_back(1);v.push_back(1);
//
//        w.addQCustomPlotData(v);
//    }
//    while(1){}
//    cout<<"123"<<endl;
//

    return a.exec();
}
