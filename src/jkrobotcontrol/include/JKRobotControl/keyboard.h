/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2017, 哈工大智能控制与系统研究所
 * All rights reserved.
 *
 * @file   keyboard.h
 * @brief  键盘控制程序
 *
 *
 * @author Weiyang LIN, Xiaoke DENG
 * @date   2017-01-07
 */
 #ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

void init_keyboard();
void close_keyboard();
int kbhit();
int readch();
int get_char();

#endif
