/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QPushButton *pushButton_2;
    QPushButton *ClearButton;
    QPushButton *pushButton_3;
    QPushButton *SaveDataButton;
    QVBoxLayout *verticalLayout_2;
    QPushButton *SaveButton;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QCustomPlot *customPlot;
    QHBoxLayout *horizontalLayout_3;
    QCustomPlot *customPlot0;
    QHBoxLayout *horizontalLayout_2;
    QCustomPlot *customPlot1;
    QCustomPlot *customPlot2;
    QVBoxLayout *verticalLayout_5;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1864, 1197);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(200, 0));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMaximumSize(QSize(16777215, 80));
        QFont font;
        font.setPointSize(14);
        pushButton_2->setFont(font);

        gridLayout->addWidget(pushButton_2, 0, 0, 1, 1);

        ClearButton = new QPushButton(groupBox);
        ClearButton->setObjectName(QString::fromUtf8("ClearButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ClearButton->sizePolicy().hasHeightForWidth());
        ClearButton->setSizePolicy(sizePolicy1);
        ClearButton->setMaximumSize(QSize(16777215, 80));
        ClearButton->setFont(font);

        gridLayout->addWidget(ClearButton, 5, 0, 1, 1);

        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setMaximumSize(QSize(16777215, 80));
        pushButton_3->setFont(font);

        gridLayout->addWidget(pushButton_3, 1, 0, 1, 1);

        SaveDataButton = new QPushButton(groupBox);
        SaveDataButton->setObjectName(QString::fromUtf8("SaveDataButton"));
        SaveDataButton->setMaximumSize(QSize(16777215, 80));
        SaveDataButton->setFont(font);

        gridLayout->addWidget(SaveDataButton, 6, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));

        gridLayout->addLayout(verticalLayout_2, 2, 0, 1, 1);

        SaveButton = new QPushButton(groupBox);
        SaveButton->setObjectName(QString::fromUtf8("SaveButton"));
        sizePolicy1.setHeightForWidth(SaveButton->sizePolicy().hasHeightForWidth());
        SaveButton->setSizePolicy(sizePolicy1);
        SaveButton->setMinimumSize(QSize(0, 0));
        SaveButton->setMaximumSize(QSize(16777215, 80));
        SaveButton->setFont(font);

        gridLayout->addWidget(SaveButton, 4, 0, 1, 1);


        horizontalLayout->addWidget(groupBox);

        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        customPlot = new QCustomPlot(widget);
        customPlot->setObjectName(QString::fromUtf8("customPlot"));
        horizontalLayout_3 = new QHBoxLayout(customPlot);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));

        horizontalLayout_4->addWidget(customPlot);

        customPlot0 = new QCustomPlot(widget);
        customPlot0->setObjectName(QString::fromUtf8("customPlot0"));

        horizontalLayout_4->addWidget(customPlot0);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        customPlot1 = new QCustomPlot(widget);
        customPlot1->setObjectName(QString::fromUtf8("customPlot1"));

        horizontalLayout_2->addWidget(customPlot1);

        customPlot2 = new QCustomPlot(widget);
        customPlot2->setObjectName(QString::fromUtf8("customPlot2"));

        horizontalLayout_2->addWidget(customPlot2);


        verticalLayout->addLayout(horizontalLayout_2);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);


        horizontalLayout->addWidget(widget);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));

        horizontalLayout->addLayout(verticalLayout_5);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 7);
        horizontalLayout->setStretch(2, 4);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1864, 28));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "\346\214\211\351\222\256\345\214\272", nullptr));
        pushButton_2->setText(QApplication::translate("MainWindow", "\345\217\257\350\247\206\345\214\226\345\214\272\345\237\237", nullptr));
        ClearButton->setText(QApplication::translate("MainWindow", "\346\270\205\351\231\244\346\233\262\347\272\277", nullptr));
        pushButton_3->setText(QApplication::translate("MainWindow", "\345\217\202\346\225\260\351\205\215\347\275\256", nullptr));
        SaveDataButton->setText(QApplication::translate("MainWindow", "\350\256\260\345\275\225\346\233\262\347\272\277\346\225\260\346\215\256", nullptr));
        SaveButton->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\346\233\262\347\272\277\345\233\276\347\211\207", nullptr));
#ifndef QT_NO_WHATSTHIS
        widget->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
