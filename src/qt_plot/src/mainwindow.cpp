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
//  绘制速度指令曲线
    connect(ROS2_QT5_Tool, &ROS2_QT5::sendMessage2Plot,this,&MainWindow::addQCustomPlotData);
//  获取视频流
    connect(ROS2_QT5_Tool, &ROS2_QT5::sendMessageImage,this,&MainWindow::importFrame);

    ROS2_QT5_thread->start();
    this->customPlotConfig();

    emit ros2_run_signal();
}







void MainWindow::customPlotConfig(){
//绘制速度指令曲线
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

//绘制Error_X曲线
    customPlot1 = ui->customPlot1;
//  添加曲线
    customPlot1->addGraph();
    customPlot1->graph(0)->setPen(QPen(QColor(40, 110, 255)));//曲线1蓝色
    customPlot1->addGraph();
    customPlot1->graph(1)->setPen(QPen(QColor(255, 110, 40)));//曲线2红色
    customPlot1->addGraph();
    customPlot1->graph(2)->setPen(QPen(QColor(110, 255, 40)));//曲线2红色
    customPlot1->addGraph();
    customPlot1->graph(3)->setPen(QPen(QColor(0, 0, 0)));//曲线2红色
    customPlot1->addGraph();
    customPlot1->graph(4)->setPen(QPen(QColor(10, 80, 225)));//曲线1蓝色
    customPlot1->addGraph();
    customPlot1->graph(5)->setPen(QPen(QColor(225, 80, 10)));//曲线2红色
    customPlot1->addGraph();
    customPlot1->graph(6)->setPen(QPen(QColor(80, 225, 10)));//曲线2红色
    customPlot1->addGraph();
    customPlot1->graph(7)->setPen(QPen(QColor(20, 255, 0)));//曲线2红色
//  设置坐标轴
    customPlot1->axisRect()->setupFullAxesBox();//四边安上坐标轴
    //设置坐标轴
    customPlot1->xAxis->setLabel("t(s)");
    customPlot1->yAxis->setLabel("error_x(m)");
    customPlot1->xAxis->setTickLabelFont(font);
    customPlot1->yAxis->setTickLabelFont(font);
    customPlot1->xAxis->setLabelFont(font);
    customPlot1->yAxis->setLabelFont(font);
    customPlot1->xAxis->ticker()->setTickCount(6);//设置网格
    customPlot1->yAxis->ticker()->setTickCount(6);//设置网格
    //设置图例
    customPlot1->legend->setFont(font);
    customPlot1->legend->setBrush(QColor(100, 100, 100, 0));
    customPlot1->graph(0)->setName("P1");
    customPlot1->graph(1)->setName("P2");
    customPlot1->graph(2)->setName("P3");
    customPlot1->graph(3)->setName("P4");
    customPlot1->graph(4)->setName("P5");
    customPlot1->graph(5)->setName("P6");
    customPlot1->graph(6)->setName("P7");
    customPlot1->graph(7)->setName("P8");
    customPlot1->legend->setVisible(true);//图例使能可视化
// 显示曲线设置
    // 使上下轴、左右轴范围同步
    connect(customPlot1->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot1->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot1->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot1->yAxis2, SLOT(setRange(QCPRange)));
    // 允许用户用鼠标拖动轴范围，用鼠标滚轮缩放，点击选择图形:
    customPlot1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

//绘制Error_SquMean曲线
    customPlot0 = ui->customPlot0;
//  添加曲线
    customPlot0->addGraph();
    customPlot0->graph(0)->setPen(QPen(QColor(0, 0, 0)));
//  设置坐标轴
    customPlot0->axisRect()->setupFullAxesBox();//四边安上坐标轴
    //设置坐标轴
    customPlot0->xAxis->setLabel("t(s)");
    customPlot0->yAxis->setLabel("error_squmean(m2)");
    customPlot0->xAxis->setTickLabelFont(font);
    customPlot0->yAxis->setTickLabelFont(font);
    customPlot0->xAxis->setLabelFont(font);
    customPlot0->yAxis->setLabelFont(font);
    customPlot0->xAxis->ticker()->setTickCount(6);//设置网格
    customPlot0->yAxis->ticker()->setTickCount(6);//设置网格
    //设置图例
    customPlot0->legend->setFont(font);
    customPlot0->legend->setBrush(QColor(100, 100, 100, 0));
    customPlot0->graph(0)->setName("SquMean");
    customPlot0->legend->setVisible(true);//图例使能可视化
// 显示曲线设置
    // 使上下轴、左右轴范围同步
    connect(customPlot0->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot1->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot0->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot1->yAxis2, SLOT(setRange(QCPRange)));

    // 允许用户用鼠标拖动轴范围，用鼠标滚轮缩放，点击选择图形:
    customPlot0->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

//绘制Error_Y曲线
    customPlot2 = ui->customPlot2;
//  添加曲线
    customPlot2->addGraph();
    customPlot2->graph(0)->setPen(QPen(QColor(40, 110, 255)));//曲线1蓝色
    customPlot2->addGraph();
    customPlot2->graph(1)->setPen(QPen(QColor(255, 110, 40)));//曲线2红色
    customPlot2->addGraph();
    customPlot2->graph(2)->setPen(QPen(QColor(110, 255, 40)));//曲线2红色
    customPlot2->addGraph();
    customPlot2->graph(3)->setPen(QPen(QColor(0, 0, 0)));//曲线2红色
    customPlot2->addGraph();
    customPlot2->graph(4)->setPen(QPen(QColor(10, 80, 225)));//曲线1蓝色
    customPlot2->addGraph();
    customPlot2->graph(5)->setPen(QPen(QColor(225, 80, 10)));//曲线2红色
    customPlot2->addGraph();
    customPlot2->graph(6)->setPen(QPen(QColor(80, 225, 10)));//曲线2红色
    customPlot2->addGraph();
    customPlot2->graph(7)->setPen(QPen(QColor(20, 255, 0)));//曲线2红色
//  设置坐标轴
    customPlot2->axisRect()->setupFullAxesBox();//四边安上坐标轴
    //设置坐标轴
    customPlot2->xAxis->setLabel("t(s)");
    customPlot2->yAxis->setLabel("error_y(m)");
    customPlot2->xAxis->setTickLabelFont(font);
    customPlot2->yAxis->setTickLabelFont(font);
    customPlot2->xAxis->setLabelFont(font);
    customPlot2->yAxis->setLabelFont(font);
    customPlot2->xAxis->ticker()->setTickCount(6);//设置网格
    customPlot2->yAxis->ticker()->setTickCount(6);//设置网格
    //设置图例
    customPlot2->legend->setFont(font);
    customPlot2->legend->setBrush(QColor(100, 100, 100, 0));
    customPlot2->graph(0)->setName("P1");
    customPlot2->graph(1)->setName("P2");
    customPlot2->graph(2)->setName("P3");
    customPlot2->graph(3)->setName("P4");
    customPlot2->graph(4)->setName("P5");
    customPlot2->graph(5)->setName("P6");
    customPlot2->graph(6)->setName("P7");
    customPlot2->graph(7)->setName("P8");
    customPlot2->legend->setVisible(true);//图例使能可视化
// 显示曲线设置
    // 使上下轴、左右轴范围同步
    connect(customPlot2->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot1->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot2->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot1->yAxis2, SLOT(setRange(QCPRange)));

    // 允许用户用鼠标拖动轴范围，用鼠标滚轮缩放，点击选择图形:
    customPlot2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);



// 按钮设置
    time = QTime(QTime::currentTime());//重置时间
    connect(ui->ClearButton, &QPushButton::clicked, this, &MainWindow::clearPlotData);//清除曲线
    connect(ui->SaveButton, &QPushButton::clicked, this, &MainWindow::saveCurseImage);//保存曲线图片
    connect(ui->SaveDataButton, &QPushButton::clicked, this, &MainWindow::saveCurseData);//保存曲线数据

////    //定时器连接槽函数realtimeDataSlot，测试程序
////    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
////    dataTimer.start(0); // 间隔时间 0ms表示尽可能快的触发
}

void MainWindow::importFrame(cv::Mat color_image,cv::Mat depth_image)
{

    cvtColor(color_image, color_image, CV_BGR2RGB);//only RGB of Qt
    QImage srcQImage1 = QImage((uchar*)(color_image.data), color_image.cols, color_image.rows, QImage::Format_RGB888);
    ui->color_image->setPixmap(QPixmap::fromImage(srcQImage1));
    ui->color_image->resize(srcQImage1.size());
    ui->color_image->show();

    QImage srcQImage2 = QImage((uchar*)(depth_image.data), depth_image.cols, depth_image.rows, QImage::Format_Indexed8);
    ui->depth_image->setPixmap(QPixmap::fromImage(srcQImage2));
    ui->depth_image->resize(srcQImage2.size());
    ui->depth_image->show();
}

void MainWindow::addQCustomPlotData(vector<float> speed_command,vector<float> error_x,vector<float> error_y,float error_squmean)
{
    if(clear_flag == true)
    {
        clear_flag = false;
        time.restart();
    }

    double key = time.elapsed()/1000.0;
//    速度指令
    for(int i = 0;i < customPlot->graphCount();i++){
        customPlot->graph(i)->addData(key, speed_command[i]);
    }
    for(int i = 0;i < customPlot->graphCount();i++){
        if(i == 0)
            customPlot->graph(i)->rescaleAxes();
        else
            customPlot->graph(i)->rescaleAxes(true);
    }
//    Error_X
    for(int i = 0;i < customPlot1->graphCount();i++){
        customPlot1->graph(i)->addData(key, error_x[i]);
    }
    for(int i = 0;i < customPlot1->graphCount();i++){
        if(i == 0)
            customPlot1->graph(i)->rescaleAxes();
        else
            customPlot1->graph(i)->rescaleAxes(true);
    }
//    Error_y
    for(int i = 0;i < customPlot2->graphCount();i++){
        customPlot2->graph(i)->addData(key, error_y[i]);
    }
    for(int i = 0;i < customPlot2->graphCount();i++){
        if(i == 0)
            customPlot2->graph(i)->rescaleAxes();
        else
            customPlot2->graph(i)->rescaleAxes(true);
    }

//    Error_SquMean
    customPlot0->graph(0)->addData(key, error_squmean);
    customPlot0->graph(0)->rescaleAxes();

    customPlot->replot();
    customPlot0->replot();
    customPlot1->replot();
    customPlot2->replot();
}

void MainWindow::saveCurseData(){
//    bool exist;
    QString fileName;
    string ss = "";
//    QDir *folder = new QDir;
//    exist = folder->exists("C:/Users/no2/Desktop");//查看目录是否存在（例子是保存到桌面）
//
//    if(!exist){//不存在就创建
//        bool ok = folder->mkdir("C:/Users/no2/Desktop");
//        if(ok){
//            QMessageBox::warning(this,tr("创建目录"),tr("创建成功!"));//添加提示方便查看是否成功创建
//        }else{
//            QMessageBox::warning(this,tr("创建目录"),tr("创建失败"));
//        }
//    }
    fileName = tr("/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/%1.txt").arg("数据");

    QFile f(fileName);
    if(!f.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)){//追加写入 添加结束符\r\n
        QMessageBox::warning(this,tr("错误"),tr("打开文件失败,数据保存失败"));
        return ;
    }else{
        for(auto iter = customPlot0->graph(0)->data()->begin();iter != customPlot0->graph(0)->data()->end();iter++)
        {
            ss += std::to_string(iter->key) + " ";
//            ss += " " + std::to_string(iter->value) + " ";
        }
        ss += '\n';
        for(auto iter = customPlot0->graph(0)->data()->begin();iter != customPlot0->graph(0)->data()->end();iter++)
        {
//            ss += std::to_string(iter->key);
            ss +=std::to_string(iter->value) + " ";
        }
        f.write(ss.c_str());

    }
    f.close();


//    cout<<customPlot0->graph(0)->data()->begin()<<endl;
    cout<<"CurseData is saved successfully!"<<endl;
}

void MainWindow::saveCurseImage(){
    static int num = 0;
    string ss = "/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/speed_command"+std::to_string(num)+".png";
    customPlot->savePng(QString(QString::fromLocal8Bit(ss.c_str())));
    ss = "/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/error_squmean"+std::to_string(num)+".png";
    customPlot0->savePng(QString(QString::fromLocal8Bit(ss.c_str())));
    ss = "/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/error_x"+std::to_string(num)+".png";
    customPlot1->savePng(QString(QString::fromLocal8Bit(ss.c_str())));
    ss = "/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/error_y"+std::to_string(num)+".png";
    customPlot2->savePng(QString(QString::fromLocal8Bit(ss.c_str())));


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


    customPlot0->graph(0)->data()->clear();
    customPlot0->xAxis->setRange(0,5);
    customPlot0->yAxis->setRange(0,5);
    clear_flag = true;
    customPlot0->replot();

    for(int i = 0;i < customPlot1->graphCount();i++){
        customPlot1->graph(i)->data()->clear();
    }
    customPlot1->xAxis->setRange(0,5);
    customPlot1->yAxis->setRange(0,5);
    clear_flag = true;
    customPlot1->replot();

    for(int i = 0;i < customPlot2->graphCount();i++){
        customPlot2->graph(i)->data()->clear();
    }
    customPlot2->xAxis->setRange(0,5);
    customPlot2->yAxis->setRange(0,5);
    clear_flag = true;
    customPlot2->replot();
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

