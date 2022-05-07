#include "qt_plot/mainwindow.h"
#include "./ui_mainwindow.h"
#include <string>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    x.resize(101);
//    y.resize(101);
//    for (int i=0; i<101; ++i)
//    {
//      x[i] = i/50.0 - 1; // x goes from -1 to 1
//      y[i] = x[i]*x[i]; // let's plot a quadratic function
//    }
//    // create graph and assign data to it:
//    ui->customPlot->addGraph();
//    ui->customPlot->graph(0)->setData(x, y);
//    // give the axes some labels:
//    ui->customPlot->xAxis->setLabel("x");
//    ui->customPlot->xAxis->ticker()->setTickCount(5);
//    ui->customPlot->yAxis->setLabel("y");
//    // set axes ranges, so we see all data:
//    ui->customPlot->xAxis->setRange(-1, 1);
//    ui->customPlot->yAxis->setRange(0, 1);
//    ui->customPlot->replot();

//            QCustomPlot* customPlot = ui->customPlot;
//        // 添加两个graph
//          customPlot->addGraph();
//          customPlot->graph(0)->setPen(QPen(Qt::blue)); // 第一条曲线颜色
//          customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // 第一条曲线和0轴围成区域填充的颜色
//          customPlot->addGraph();
//          customPlot->graph(1)->setPen(QPen(Qt::red)); // 第二条曲线颜色
//          // 生成数据
//          QVector<double> x(251), y0(251), y1(251);
//          for (int i=0; i<251; ++i)
//          {
//            x[i] = i;
//            y0[i] = qExp(-i/150.0)*qCos(i/10.0); // 指数衰减的cos
//            y1[i] = qExp(-i/150.0);              // 衰减指数
//          }
//          // 配置右侧和顶部轴显示刻度，但不显示标签:
//          customPlot->xAxis2->setVisible(true);
//          customPlot->xAxis2->setTickLabels(false);
//          customPlot->yAxis2->setVisible(true);
//          customPlot->yAxis2->setTickLabels(false);
//          // 让左边和下边轴与上边和右边同步改变范围
//          connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
//          connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
//          // 设置数据点
//          customPlot->graph(0)->setData(x, y0);
//          customPlot->graph(1)->setData(x, y1);
//          // 让范围自行缩放，使图0完全适合于可见区域:
//          customPlot->graph(0)->rescaleAxes();
//          // 图1也是一样自动调整范围，但只是放大范围(如果图1小于图0):
//          customPlot->graph(1)->rescaleAxes(true);
//          // 允许用户用鼠标拖动轴范围，用鼠标滚轮缩放，点击选择图形:
//          customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
//          customPlot->replot();
     //坐标轴使用时间刻度
//     QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//     timeTicker->setTimeFormat("%s");
//     customPlot->xAxis->setTicker(timeTicker);
    ROS2_QT5_Tool = new ROS2_QT5();
    ROS2_QT5_thread = new QThread();
    ROS2_QT5_Tool->moveToThread(ROS2_QT5_thread);
    connect(this, &MainWindow::ros2_run_signal,ROS2_QT5_Tool,&ROS2_QT5::ros2_run);
    connect(ROS2_QT5_Tool, &ROS2_QT5::sendSpeedCommand,this,&MainWindow::addQCustomPlotData);

    ROS2_QT5_thread->start();
    this->customPlotConfig();

    emit ros2_run_signal();
}

void MainWindow::customPlotConfig(){
    customPlot = ui->customPlot;
    QFont font;
    font.setFamily("微软雅黑");//字体
    font.setPixelSize(20);//文字像素大小
//  添加曲线
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));//曲线1蓝色
    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));//曲线2红色
    customPlot->addGraph();
    customPlot->graph(2)->setPen(QPen(QColor(110, 255, 40)));//曲线2红色
    customPlot->addGraph();
    customPlot->graph(3)->setPen(QPen(QColor(0, 0, 0)));//曲线2红色
//  设置坐标轴
    customPlot->axisRect()->setupFullAxesBox();//四边安上坐标轴
    //设置坐标轴
    customPlot->xAxis->setLabel("t(s)");
    customPlot->yAxis->setLabel("v(cm/s) | ω(rad/s)");
    customPlot->xAxis->setTickLabelFont(font);
    customPlot->yAxis->setTickLabelFont(font);
    customPlot->xAxis->setLabelFont(font);
    customPlot->yAxis->setLabelFont(font);
    customPlot->xAxis->ticker()->setTickCount(6);//设置网格
    customPlot->yAxis->ticker()->setTickCount(6);//设置网格
    //设置图例
    customPlot->legend->setFont(font);
    customPlot->legend->setBrush(QColor(100, 100, 100, 0));
    customPlot->graph(0)->setName("Vx");
    customPlot->graph(1)->setName("Vy");
    customPlot->graph(2)->setName("Vz");
    customPlot->graph(3)->setName("ωz");
    customPlot->legend->setVisible(true);//图例使能可视化
// 显示曲线设置
    // 使上下轴、左右轴范围同步
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
    // 允许用户用鼠标拖动轴范围，用鼠标滚轮缩放，点击选择图形:
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
// 按钮设置
    time = QTime(QTime::currentTime());//重置时间
    connect(ui->ClearButton, &QPushButton::clicked, this, &MainWindow::clearPlotData);//清除曲线
    connect(ui->SaveButton, &QPushButton::clicked, this, &MainWindow::saveCurseImage);//保存曲线图片
////    //定时器连接槽函数realtimeDataSlot，测试程序
////    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
////    dataTimer.start(0); // 间隔时间 0ms表示尽可能快的触发
}

void MainWindow::addQCustomPlotData(vector<float> y_data)
{
    if(clear_flag == true)
    {
        clear_flag = false;
        time.restart();
    }

    double key = time.elapsed()/1000.0;
    for(int i = 0;i < customPlot->graphCount();i++){
        customPlot->graph(i)->addData(key, y_data[i]);
    }
    for(int i = 0;i < customPlot->graphCount();i++){
        if(i == 0)
            customPlot->graph(i)->rescaleAxes();
        else
            customPlot->graph(i)->rescaleAxes(true);
    }
    customPlot->replot();
}

void MainWindow::saveCurseImage(){
    static int num = 0;
    string ss = "/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/"+std::to_string(num)+".png";

    customPlot->savePng(QString(QString::fromLocal8Bit(ss.c_str())));
    cout<<"Curse is saved successfully!"<<endl;
    num++;
}

void MainWindow::clearPlotData(){
    for(int i = 0;i < customPlot->graphCount();i++){
        customPlot->graph(i)->data()->clear();
    }
    customPlot->xAxis->setRange(0,5);
    customPlot->yAxis->setRange(0,5);
    clear_flag = true;
    customPlot->replot();
}

void MainWindow::realtimeDataSlot()
{
        QCustomPlot* customPlot = ui->customPlot;
       static QTime time(QTime::currentTime());

       double key = time.elapsed()/1000.0; // 开始到现在的时间，单位秒
       static double lastPointKey = 0;
       if (key-lastPointKey > 0.002) // 大约2ms添加一次数据
       {
         // 添加数据到graph
         customPlot->graph(0)->addData(key, qSin(key)+qrand()/(double)RAND_MAX*1*qSin(key/0.3843));
         customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
         customPlot->graph(2)->addData(key, qSin(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.3843));
         customPlot->graph(3)->addData(key, qSin(key)+qrand()/(double)RAND_MAX*1.0*qSin(key/0.4364));
         //记录当前时刻
         lastPointKey = key;
       }
       // 曲线能动起来的关键在这里，设定x轴范围为最近8个时刻
//       customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
       customPlot->graph(0)->rescaleAxes();
       customPlot->graph(1)->rescaleAxes(true);
        customPlot->graph(2)->rescaleAxes(true);
        customPlot->graph(3)->rescaleAxes(true);
       //绘图
       customPlot->replot();
       //计算帧数
       static double lastFpsKey;
       static int frameCount;
       ++frameCount;
       if (key-lastFpsKey > 2) // 每2秒求一次平均值
       {
           //状态栏显示帧数和数据总数
//         ui->statusBar->showMessage(
//               QString("%1 FPS, Total Data points: %2")
//               .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
//               .arg(customPlot->graph(0)->data()->size()+customPlot->graph(1)->data()->size())
//               , 0);
         lastFpsKey = key;
         frameCount = 0;
       }

}


MainWindow::~MainWindow()
{
    delete ui;
}

