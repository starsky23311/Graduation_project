#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qt_plot/qcustomplot.h"
#include <vector>
#include <chrono>
#include <memory>
#include <iostream>
//ROS
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DataReceiver : public rclcpp::Node
{
public:
    DataReceiver()
            : Node("DataReceiverNode")
    {

        this->subscription_speedcommand = this->create_subscription<std_msgs::msg::String>(
                "SpeedCommand", 5, std::bind(&DataReceiver::speedcommand_callback, this, std::placeholders::_1));
        this->v.resize(6);
        flag = false;
    }
    vector<float> getSpeedCommand()
    {
        return this->v;
    }
    bool flag;
private:
//速度指令
    vector<float> v;

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_speedcommand;

    void setSpeedCommand(const string& msg){
        if(*msg.begin() == '[' && *(msg.end()-1) == ']') {
            string s1(msg.begin()+1,msg.end()-1),s2;
            stringstream ss(s1);int num = 0;
            while(getline(ss, s2, ',')) {

                this->v[num] = std::atof(s2.c_str());
                num++;

            }
        }
        else{
            cout <<"Error MSG Of Grasp!"<<endl;
            return ;
        }
    }

    void speedcommand_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "I heard EncoderDepth: %s",msg->data.c_str());
        this->setSpeedCommand(msg->data.c_str());
        flag = true;
//        this->encoder_depth = std::atof(msg->data.c_str());
    }
};

class ROS2_QT5 : public QObject
{
Q_OBJECT
public:
    ROS2_QT5(QWidget *parent = nullptr){

        qRegisterMetaType<vector<float>>("vector<float>");


    }
    ~ROS2_QT5(){}
//    void addQCustomPlotData(vector<float> y_data);

//private slots:
//    void realtimeDataSlot();
signals:
    void sendSpeedCommand(vector<float> speed_command);
public slots:
    void ros2_run(){
        char **argv=NULL;
        rclcpp::init(0, argv);
        DataReceiverTool = std::make_shared<DataReceiver>() ;
//        rclcpp::WallRate loop_rate(10);
    while(rclcpp::ok){
        rclcpp::spin_some(DataReceiverTool);
        if(DataReceiverTool->flag) {
            DataReceiverTool->flag = false;
            emit sendSpeedCommand(DataReceiverTool->getSpeedCommand());
        }
//        loop_rate.sleep();
    }

        rclcpp::shutdown();
    }
private:
    std::shared_ptr<DataReceiver> DataReceiverTool;
//    Ui::MainWindow *ui;
//    QCustomPlot* customPlot;
//    QTimer dataTimer;

//    void customPlotConfig();

};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void ros2_run_signal();

private slots:
    void realtimeDataSlot();
    void addQCustomPlotData(vector<float> y_data);
    void saveCurseImage();
    void clearPlotData();
private:
    Ui::MainWindow *ui;
    QTime time;
    bool clear_flag = true;
    QCustomPlot* customPlot;
    QTimer dataTimer;
    ROS2_QT5* ROS2_QT5_Tool;
    QThread* ROS2_QT5_thread;
    void customPlotConfig();

};






#endif // MAINWINDOW_H
