#ifndef _ROBOT_STATE_H
#define _ROBOT_STATE_H

#include <stdio.h>
#include <math.h>
#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <unistd.h>  //headers for Tcp
#include <iostream>
#include <sstream>
#include <Eigen/Dense>
#include <Eigen/Eigen>

#include "JKRobotControl/TCPServer.h"
#include "JKRobotControl/TCPClient.h"

using namespace std;
using namespace Eigen;

#define  Vector6d Matrix<double,6,1>

typedef enum {
  BOOL_TYPE,
  INT_TYPE,
  DOUBLE_TYPE
} DATA_TYPE;

class robotState {
 public:
  Vector6d cEnd;
  Vector6d curJoint;
  Vector6d curJointVel;
  Vector6d coeffJv;

  robotState();
  int updateRobotState();
  int printRobotState();
  float PosuJntVel(Vector6d curJnt, Vector6d targJnt, float Jvl);

  TCPClient* tcp;
  int getTcpPointer(TCPClient* _tcp);

  int getOneParameter(const char* cmdStr, void* value, DATA_TYPE type);
  int setOneParameter(const char* cmdStr, void* value, DATA_TYPE type);

  int getMultiParameters(const char* cmdStr,
			 void* value, int dim, DATA_TYPE type);
  int setMultiParameters(const char* cmdStr,
			 void* value, int dim, DATA_TYPE type);

  /******************* get status *********************/
	int getEnableState(int* Enable);
  int getJoints(Vector6d& joints) ;
  int getJointsVel(Vector6d& joints);
  int getEndPos(Vector6d& Pos);

  int getMode(int* mode);
  int getPvr(double* ratio);
  int getJvr(double* ratio);
  int getSwitchFlag4(int* flag);//YUNXING button

  int getRelMovAndFrameFlag(int* relFlag, int *frameFlag);  // 0 is
  int getJointMovStatusFlag(int* flag);// 0 is
  int getPosMovStatusFlag(int* flag);// 0 is
  int getCircleMovStatusFlag(int* flag);// 0 is

  int getForceTouchFlag(int* flag);//GETFTF, int
  int getForceTouchThres(Vector6d& thres);  //  GETFORCE, double, 6
  int getCarForceFeedFroward(Vector6d& ForceForward);
  int getCartesianImpCtr(int* flag);
  int getKX(Vector6d& KX);  // 获取笛卡尔空间阻抗刚度
  int getKV(Vector6d& KV);  //  设置笛卡尔空间zuni
  int getP7E(Vector3d& P7E);
  int getLoadParas(Vector4d& LoadParas);
  /********************  Virtual wall **************************/
  // +x, +y, +z, -x, -y, -z
  int setVirtualWallLimit(Vector6d &VirWallLimits);
  int setVirtualWallAngle(double angle);
  int setVirtualWallEnable(int flag);
/******************* set status *********************/

  int setEnable(int enable);
  int setMode(int mode);
  int setPvr(double ratio);
  int setJvr(double ratio);
  int setSwitchFlag4(int flag);  // YUNXING button
  int setRelMovAndFrameFlag(int relFlag, int frameFlag);  // 0 is
  int setSafetyIndex(double ratio);  // 0 is
  int setCartesianImpCtr(int flag);
  int setForcelimitProtect(int flag);
  int setP7E(Vector3d& P7E);
  int setLoadParas(Vector4d &LoadParas);

  int setModeOCCM(double MOCCM);
  int setROCCM(double rOCCM);
  int setSelectOPos(double select_O_pos);
  int setSelectDirection(double select_direction);
  int setForceReset(int flag);

/******************* set pos joints and torque *********************/
  int setJoints(const Vector6d& joints);
  int setJointsVel(const Vector6d& joints);
  int setJvs(double ratio);
  int setPos(const Vector6d& Pos);
  int setEndVel(const Vector6d &vel);//------
  int setTotalVel(double m_vel);
  int setForceTouchFlag(int flag);  // SETFTF
  int setForceTouchThres(const Vector6d& thres);  // SETFORCE
  int setCarForceFeedForward(const Vector6d &ForceForward);
  int setKX(const Vector6d &kx);  // 设置笛卡尔空间刚度
  int setKV(const Vector6d &kv);  // 设置笛卡尔空间zuni

/***************  middle level API *****************/
  //  block means wait until finish work or not
  int movJoint(const Vector6d& joints, double Jvr,
							 int relative, int block);
  int movJointYoumen(const Vector6d& joints, double Jvr,
										 int relative, int block);
  int movPosAndForceLimit(const Vector6d& pos,
			  double Pvr, Vector6d Forcelimits,
				Vector6d Forcesensor, int frame, int relative);
  int movJointAndForceLimit(const Vector6d& joints,
			    double Jvr, Vector6d Forcelimits,
					Vector6d Forcesensor, int frame, int relative);
  int movPosAndImpCtrl(const Vector6d& pos, double Pvr,
											 Vector6d KX, Vector6d KV,
											 Vector6d forwardforce, int frame,
											 int relative);
  int movJointAndImpCtrl(const Vector6d& joints, double Pvr,
												 Vector6d KX, Vector6d KV,
												 Vector6d forwardforce, int frame,
												 int relative);
  // frame = 1 means tool frame, relative = 1 means relative mov
  int movCircle(const Vector6d& targetPos, double mode_OCCM,
			  double radius,
			  double select_O_pos, double select_direction,
			  double Pvr);
  //  mode_OCCM = 0 :radius,
  //  mode_OCCM = 1 :circle point
  //  select_O_pos = 0 : circle point under the line
  //  select_O_pos = 1 : circle point above the line
  //  select_directio = 0 : smaller arc
  //  select_directio = 1 : bigger arc
  /***************  high level API *****************/
  // for compatible with TM program
  int motionInit();  // set init value
	int motionInit(float safeindex, float Jvr, float Pvr);
  int movAbsBase(const Vector6d& point);
  int movAbsJoint(const Vector6d& joint);
  int movAbsJoint(const Vector6d& joint, float Jvl);
  int movAbsJointYoumen(const Vector6d& joint, float Jvl);
  int movRelJoint(const Vector6d& joint);
  int movRelJoint(const Vector6d& joint, float Jvl);
  int movRelTool(const Vector6d& pos, double Pvr);

  int pushButton(Vector6d force, Vector6d movPos,
			   Vector6d FS_thres, double pvr);
  double *pForceSensor, *pInitForceSensor;
  int setInitSensorForce();
  int getForceSensorFlag(Vector6d forceThreshold);
};

#endif