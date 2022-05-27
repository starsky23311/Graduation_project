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
        this->subscription_error_xy = this->create_subscription<std_msgs::msg::String>(
                "ErrorXY", 5, std::bind(&DataReceiver::error_xy_callback, this, std::placeholders::_1));

        this->v.resize(6);
        this->error_x.resize(8);
        this->error_y.resize(8);
        flag1 = false;
        flag2 = false;
    }
    vector<float> getSpeedCommand()
    {
        return this->v;
    }
    vector<float> getErrorX()
    {
        return this->error_x;
    }
    vector<float> getErrorY()
    {
        return this->error_y;
    }
    float getErrorSquMean()
    {
        return error_squmean;
    }
    bool flag1;
    bool flag2;
private:
//速度指令
    vector<float> v;
    vector<float> error_x;
    vector<float> error_y;
    float error_squmean;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_speedcommand;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_error_xy;

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
            cout <<"Error MSG Of Speed!"<<endl;
            return ;
        }
    }
    void setErrorXY(const string& msg){
        if(*msg.begin() == '[' && *(msg.end()-1) == ']') {
            string s1(msg.begin()+1,msg.end()-1),s2;
            stringstream ss(s1);int num = 0;
            while(getline(ss, s2, ',')) {

                if(num % 2 == 0 && num != 16)
                    this->error_x[num] = std::atof(s2.c_str());
                else if(num % 2 == 1 && num != 16)
                    this->error_y[num] = std::atof(s2.c_str());
                else
                    error_squmean = std::atof(s2.c_str());

                num++;

            }
        }
        else{
            cout <<"Error MSG Of ErrorXY!"<<endl;
            return ;
        }
    }
    void speedcommand_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "I heard SpeedCommand: %s",msg->data.c_str());
        this->setSpeedCommand(msg->data.c_str());
        flag1 = true;
//        this->encoder_depth = std::atof(msg->data.c_str());
    }
    void error_xy_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "I heard ErrorXY: %s",msg->data.c_str());
        this->setErrorXY(msg->data.c_str());
        flag2 = true;
//        this->encoder_depth = std::atof(msg->data.c_str());
    }
};

class ROS2_QT5 : public QObject
{
Q_OBJECT
public:
    ROS2_QT5(QWidget *parent = nullptr){

        qRegisterMetaType<vector<float>>("vector<float>");
        qRegisterMetaType<float>("float");

    }
    ~ROS2_QT5(){}
//    void addQCustomPlotData(vector<float> y_data);

//private slots:
//    void realtimeDataSlot();
signals:
    void sendMessage2Plot(vector<float> speed_command,vector<float> error_x,vector<float> error_y,float error_squmean);
public slots:
    void ros2_run(){
        char **argv=NULL;
        rclcpp::init(0, argv);
        DataReceiverTool = std::make_shared<DataReceiver>() ;
//        rclcpp::WallRate loop_rate(10);
    while(rclcpp::ok){
        rclcpp::spin_some(DataReceiverTool);
        if(DataReceiverTool->flag1 && DataReceiverTool->flag2) {
            DataReceiverTool->flag1 = false;
            DataReceiverTool->flag2 = false;
            emit sendMessage2Plot(DataReceiverTool->getSpeedCommand(),DataReceiverTool->getErrorX(),DataReceiverTool->getErrorY(),DataReceiverTool->getErrorSquMean());
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
    void addQCustomPlotData(vector<float> speed_command,vector<float> error_x,vector<float> error_y,float error_squmean);
    void saveCurseData();
    void saveCurseImage();
    void clearPlotData();

private:
    Ui::MainWindow *ui;
    QTime time;
    bool clear_flag = true;
    QCustomPlot* customPlot;
    QCustomPlot* customPlot0;
    QCustomPlot* customPlot1;
    QCustomPlot* customPlot2;
    QTimer dataTimer;
    ROS2_QT5* ROS2_QT5_Tool;
    QThread* ROS2_QT5_thread;
    void customPlotConfig();

};






#endif // MAINWINDOW_H
