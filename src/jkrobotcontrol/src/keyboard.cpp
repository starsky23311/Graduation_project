/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2017, ���������ܿ�����ϵͳ�о���
 * All rights reserved.
 *
 * @file   keyboard.cpp
 * @brief  ���̿��Ƴ���
 *
 *
 * @author Weiyang LIN, Xiaoke DENG
 * @date   2017-01-07
 */
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include "JKRobotControl/keyboard.h"

#define TTY_PATH            "/dev/tty"
#define STTY_US             "stty raw -echo -F "
#define STTY_DEF            "stty -raw echo -F "


/**
 * @brief  ��ʼ�����̿��Ƴ���
 *
 */
void init_keyboard()
{
  system(STTY_US TTY_PATH);
}

/**
 * @brief  �رռ��̿��Ƴ���
 *
 */
void close_keyboard()
{
  system(STTY_DEF TTY_PATH);
}

/**
 * @brief  ��ȡ��������ֵ
 *
 * @return int ������
 */
int get_char() {
  fd_set rfds;
  struct timeval tv;
  int ch = -1;

  FD_ZERO(&rfds);
  FD_SET(0, &rfds);
  tv.tv_sec = 0;
  tv.tv_usec = 10; //���õȴ���ʱʱ��

  //�������Ƿ�������
  if (select(1, &rfds, NULL, NULL, &tv) > 0)
    ch = getchar();

  return ch;
}
