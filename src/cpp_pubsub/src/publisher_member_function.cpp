//// Copyright 2016 Open Source Robotics Foundation, Inc.
////
//// Licensed under the Apache License, Version 2.0 (the "License");
//// you may not use this file except in compliance with the License.
//// You may obtain a copy of the License at
////
////     http://www.apache.org/licenses/LICENSE-2.0
////
//// Unless required by applicable law or agreed to in writing, software
//// distributed under the License is distributed on an "AS IS" BASIS,
//// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//// See the License for the specific language governing permissions and
//// limitations under the License.
//
#include <chrono>
#include <memory>
#include <iostream>
#include <fstream>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <yaml-cpp/yaml.h>
using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher()
  : Node("minimal_publisher"), count_(0)
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    timer_ = this->create_wall_timer(
      500ms, std::bind(&MinimalPublisher::timer_callback, this));
    this->getConfigParam();
  }

private:
  void getConfigParam()
  {
      std::string fin = "/home/wzf/main_folder/MyProject/Graduation_project/src/cpp_pubsub/config/talker.yaml";       //yaml文件所在的路径
      YAML::Node yamlConfig = YAML::LoadFile(fin);
      int int_param = yamlConfig["int_param"].as<int>();
      std::cout << "  node size: " << yamlConfig.size() << std::endl;
      std::cout << yamlConfig["bool_param"].as<bool>() << "\n";
      yamlConfig["bool_param"] = !yamlConfig["bool_param"].as<bool>();
      yamlConfig["str_param"] = "test";
      std::ofstream file;
      file.open(fin);
      file.flush();
      file << yamlConfig;
      file.close();

  }



  void timer_callback()
  {
    auto message = std_msgs::msg::String();
//    message.data = "Hello, world! " + std::to_string(count_++);
    message.data = "[" + std::to_string(count_++) + "," + std::to_string(count_+1) + "," + std::to_string(count_+2) + "," + std::to_string(count_+3) + "]" ;
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher_->publish(message);
  }


  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  size_t count_;
};

int main(int argc, char * argv[])
{

  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}




//mainwindow.h

//#include <QApplication>
//#include <QMainWindow>
//#include <QLabel>      // 引入 QLable 文件框组件的头文件
//class MainWindow : public QMainWindow
//{
//Q_OBJECT
//public:
//    MainWindow(QWidget *parent = 0);
//    ~MainWindow();
//private:
//    QLabel *lab;        // 定义一个私有的 QLabel 指针对象
//};
//
//MainWindow::MainWindow(QWidget *parent)
//        : QMainWindow(parent)
//{
//    // 创建一个 QLable 对象
//    this->lab = new QLabel("Hello,World!",this);
//}
//MainWindow::~MainWindow()
//{
//}
//
//int main(int argc, char * argv[]){
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
//}
//#include "/home/wzf/main_folder/MyProject/Graduation_project/src/cpp_pubsub/cmake-build-debug/talker_autogen/include/publisher_member_function.moc"