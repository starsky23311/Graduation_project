/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2019, 哈工大智能控制与系统研究所
 * All rights reserved.
 *
 * @file   main.h
 * @brief  主程序
 *
 * @author Weiyang LIN, Liu Chenlu
 * @date   2019-11-12
 *
 */
#ifndef _MAIN_H
#define _MAIN_H

#include <iostream>
#include "JKRobotControl/keyboard.h"
//#include "robotState.h"
#include "JKRobotControl/robotState.h"
#include "JKRobotControl/TCPClient.h"
using namespace std;

int CheckKeyEvent();

robotState m_robot;
TCPClient tcp1;

#endif
