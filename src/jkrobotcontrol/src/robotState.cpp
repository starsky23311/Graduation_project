/**
 * @file   robotState.cpp
 * @brief
 *
 * <long description>
 *
 * @author Liu Chenlu
 * @date   2019-05-21
 */
#include "JKRobotControl/robotState.h"
#include <string>
#include <vector>

#define endr "\n\r"
robotState::robotState() {
  coeffJv << 1, 1, 1, 1, 1, 1;
}


int SplitString(const string &s, vector<string> &v, const string &c) {
  string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  v.clear();
  while (string::npos != pos2) {
    v.push_back(s.substr(pos1, pos2 - pos1));
    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if (pos1 != s.length())
    v.push_back(s.substr(pos1));
  return v.size();
}

int robotState::updateRobotState() {
  getJoints(curJoint);
  getJointsVel(curJointVel);
  getEndPos(cEnd);
  return 1;
}

int robotState::printRobotState() {
  cout << "End pos: " << cEnd.transpose() << endr
       << "Joint pos: " << curJoint.transpose() << endr;
  return 0;
}

int robotState::getTcpPointer(TCPClient *_tcp) {
  tcp = _tcp;
  return 1;
}

/**************** general method *********************/
int robotState::getOneParameter(const char *cmdStr, void *value,
                                DATA_TYPE type) {
  vector<string> splitStrs;

  tcp->Send(cmdStr);
  string str = tcp->receive(1000);
  int n = SplitString(str, splitStrs, " ");
  if (n != 2)
    return -1;
  if (strcasecmp(splitStrs[0].c_str(), cmdStr) != 0)
    return 0;
  switch (type) {
    case BOOL_TYPE: {
      int *p = (int *)value;
      if (splitStrs[1] == "0") {
	*p = 0;
      } else if (splitStrs[1] == "1") {
	*p = 1;
      } else
	return 0;
      break;
    }
    case INT_TYPE: {
      int *p = (int *)value;
      *p = atoi(splitStrs[1].c_str());
      break;
    }
    case DOUBLE_TYPE: {
      double *p = (double *)value;
      *p = atof(splitStrs[1].c_str());
      break;
    }
    default:
      return 0;
      break;
  }
  return 1;
}

int robotState::setOneParameter(const char *cmdStr, void *value,
                                DATA_TYPE type) {
  vector<string> splitStrs;
  char cmd[256] = "";

  switch (type) {
    case BOOL_TYPE: {
      int *p = (int *)value;
      sprintf(cmd, "%s %d", cmdStr, *p);
      break;
    }
    case INT_TYPE: {
      int *p = (int *)value;
      sprintf(cmd, "%s %d", cmdStr, *p);
      break;
    }
    case DOUBLE_TYPE: {
      double *p = (double *)value;
      sprintf(cmd, "%s %lf", cmdStr, *p);
      break;
    }
    default:
      return 0;
      break;
  }
  tcp->Send(cmd);

  string str = tcp->receive(1000);

  int n = SplitString(str, splitStrs, " ");
  if (n != 2)
    return -1;
  if (strcasecmp(splitStrs[0].c_str(), cmdStr) != 0)
    return 0;
  if (strcasecmp(splitStrs[1].c_str(), "OK") != 0)
    return 0;

  return 1;
}

int robotState::getMultiParameters(const char *cmdStr, void *value, int dim,
                                   DATA_TYPE type) {
  vector<string> splitStrs;
  if (dim <= 0) return 0;

  tcp->Send(cmdStr);
  string str = tcp->receive(1000);

  //cout << "recStr: "<<str<<endr;

  int n = SplitString(str, splitStrs, " ");
  if (n != dim + 1)
    return -1;
  if (strcasecmp(splitStrs[0].c_str(), cmdStr) != 0)
    return 0;
  switch (type) {
    case BOOL_TYPE: {
      int *p = (int *)value;
      for (int i = 0; i < dim; i++) {
	if (splitStrs[i + 1] == "0") {
	  p[i] = 0;
	} else if (splitStrs[i + 1] == "1") {
	  p[i] = 1;
	} else
	  return 0;
      }
      break;
    }
    case INT_TYPE: {
      int *p = (int *)value;
      for (int i = 0; i < dim; i++)
	p[i] = atoi(splitStrs[i + 1].c_str());
      break;
    }
    case DOUBLE_TYPE: {
      double *p = (double *)value;
      for (int i = 0; i < dim; i++)
	p[i] = atof(splitStrs[i + 1].c_str());
      break;
    }

    default:
      return 0;
      break;
  }
  return 1;
}

int robotState::setMultiParameters(const char *cmdStr, void *value,
																	 int dim, DATA_TYPE type) {
  vector<string> splitStrs;
  string cmd = "";
  char str[256] = "";

  if (dim <= 0) return 0;

  switch (type) {
    case BOOL_TYPE: {
      int *p = (int *)value;
      cmd = cmdStr;
      for (int i = 0; i < dim; i++) {
	sprintf(str, " %d", p[i]);
	cmd += str;
      }
      break;
    }
    case INT_TYPE: {
      int *p = (int *)value;
      cmd = cmdStr;
      for (int i = 0; i < dim; i++) {
	sprintf(str, " %d", p[i]);
	cmd += str;
      }
      break;
    }
    case DOUBLE_TYPE: {
      double *p = (double *)value;
      cmd = cmdStr;
      for (int i = 0; i < dim; i++) {
	sprintf(str, " %lf", p[i]);
	cmd += str;
      }
      break;
    }
    default:
      return 0;
      break;
  }
  tcp->Send(cmd);

  string str2 = tcp->receive(1000);

  int n = SplitString(str2, splitStrs, " ");
  if (n != 2)
    return -1;
  if (strcasecmp(splitStrs[0].c_str(), cmdStr) != 0)
    return 0;
  if (strcasecmp(splitStrs[1].c_str(), "OK") != 0)
    return 0;

  return 1;
}
/******************* get status *********************/
int robotState::getEnableState(int* Enable) {
  if (getOneParameter("GETEnable", Enable, INT_TYPE) == 0)
    return 0;
  else
    cout << "GETEnable : OK" << endr;
  return 1;
}

int robotState::getJoints(Vector6d& joints) {
  double value[6];
  if (getMultiParameters("GETJ", value, 6, DOUBLE_TYPE) == 0) {
    return 0;
  } else {
    for (int i =0; i< 6; i++) {
      joints[i] = value[i];
    }
  }
  // cout << "GETJ: OK" << joints.transpose() << endr;
  return 1;
}

int robotState::getJointsVel(Vector6d &joints) {
  double value[6];
  if (getMultiParameters("GETJV", value, 6, DOUBLE_TYPE) == 0)
    return 0;
  else {
    for (int i =0; i< 6; i++) joints[i] = value[i];
  }
  //  cout << "GETJV: OK" << joints.transpose() << endr;
  return 1;
}

int robotState::getEndPos(Vector6d &pos) {
  double value[6];
  if (getMultiParameters("GETP", value, 6, DOUBLE_TYPE) == 0)
    return 0;
  else {
    for (int i =0; i< 6; i++) pos[i] = value[i];
  }
  //  cout << "GETP: OK" << pos.transpose() << endr;
  return 1;
}

int robotState::getMode(int* mode) {
  if (getOneParameter("GETM", mode, INT_TYPE) == 0)
    return 0;
  else
    cout << "GETM : OK" << endr;
  return 1;
}

int robotState::getPvr(double* ratio) {
  if (getOneParameter("GETPVR", ratio, DOUBLE_TYPE) == 0)
    return 0;
  else
    cout << "GETPVR : OK" << endr;
  return 1;
}

int robotState::getJvr(double* ratio) {
  if (getOneParameter("GETJVR", ratio, DOUBLE_TYPE) == 0)
    return 0;
  else
    cout << "GETJVR : OK" << endr;
  return 1;
}
int robotState::getSwitchFlag4(int* flag) {
  if (getOneParameter("GETSF4", flag, INT_TYPE) == 0)
    return 0;
  return 1;
}
int robotState::getRelMovAndFrameFlag(int* relFlag, int *frameFlag) {
  int value[2];
  if (getMultiParameters("GETFM", value, 2, INT_TYPE) == 0)
    return 0;
  else
  {
    *frameFlag = value[0];
    *relFlag = value[1];
  }
  cout << "GETFM2 : OK" << *relFlag<<*frameFlag << endr;
  return 1;
}

int robotState::getJointMovStatusFlag(int* flag) {
  if (getOneParameter("GETJF", flag, INT_TYPE) == 0)
    return 0;
  return 1;
}
int robotState::getPosMovStatusFlag(int* flag) {
  if (getOneParameter("GETPF", flag, INT_TYPE) == 0)
    return 0;
   // cout << "GETPF : OK"<< *flag << endr;
  return 1;
}

int robotState::getCircleMovStatusFlag(int* flag) {
  if (getOneParameter("GETCF", flag, INT_TYPE) == 0)
    return 0;
  return 1;
}

int robotState::getCartesianImpCtr(int* flag) {
  if (getOneParameter("GETCIC", flag, INT_TYPE) == 0)
    return 0;
  return 1;
}

int robotState::getForceTouchFlag(int* flag)
{
  if (getOneParameter("GETFTF", flag, INT_TYPE) == 0)
    return 0;
    // cout << "GETFTF : OK"<< *flag << endr;
  return 1;
}
int robotState::getForceTouchThres(Vector6d &thres) {
  double value[6];
  if (getMultiParameters("GETFORCE", value, 6, DOUBLE_TYPE) == 0)
    return 0;
  else {
    for (int i =0; i< 6; i++)
      thres[i] = value[i];
  }
  cout << "GETFORCE: OK" << thres.transpose() << endr;
  return 1;
}
int robotState::getCarForceFeedFroward(Vector6d& ForceForward) {
  double value[6];
  if (getMultiParameters("GETCFFF", value, 6, DOUBLE_TYPE) == 0) {
    return 0;
  } else {
    for (int i =0; i< 6; i++) {
      ForceForward[i] = value[i];
    }
  }
  cout << "GETCFFF: OK" << ForceForward.transpose() << endr;
  return 1;
}
// 获取笛卡尔空间刚度
int robotState::getKX(Vector6d& KX) {
  double value[6];
  if (getMultiParameters("GETKX", value, 6, DOUBLE_TYPE) == 0) {
    return 0;
  } else {
    for (int i =0; i< 6; i++) {
      KX[i] = value[i];
    }
  }
  cout << "GETKX: OK" << KX.transpose() << endr;
  return 1;
}
//  设置笛卡尔空间zuni
int robotState::getKV(Vector6d& KV) {
  double value[6];
  if (getMultiParameters("GETKV", value, 6, DOUBLE_TYPE) == 0) {
    return 0;
  } else {
    for (int i =0; i< 6; i++) {
      KV[i] = value[i];
    }
  }
  cout << "GETKV: OK" << KV.transpose() << endr;
  return 1;
}

int robotState::getP7E(Vector3d& P7E) {
  double value[3];
  if (getMultiParameters("GETP7E", value, 3, DOUBLE_TYPE) == 0) {
    return 0;
  } else {
    for (int i =0; i< 3; i++) {
      P7E[i] = value[i];
    }
  }
  cout << "GETP7E: OK" << P7E.transpose() << endr;
  return 1;
}

int robotState::getLoadParas(Vector4d& LoadParas) {
  double value[4];
  if (getMultiParameters("GETLoadParas", value, 4, DOUBLE_TYPE) == 0) {
    return 0;
  } else {
    for (int i =0; i< 4; i++) {
      LoadParas[i] = value[i];
    }
  }
  cout << "GETLoadParas: OK" << LoadParas.transpose() << endr;
  return 1;
}
/******************* set status *********************/

int robotState::setMode(int mode)
{
  if (setOneParameter("SETM", &mode, INT_TYPE) == 0)
    return 0;
  // else
  //   cout << "SETM : OK" << endr;
  return 1;
}
int robotState::setPvr(double ratio) {
  if (setOneParameter("SETPVR", &ratio, DOUBLE_TYPE) == 0)
    return 0;
  // else
  //   cout << "SETPVR : OK" << endr;
  return 1;
}
int robotState::setEnable(int enable) {
  if (setOneParameter("SETENABLE", &enable, INT_TYPE) == 0) return 0;
  else
    cout << "SETENABLE : OK" << endr;
  return 1;
}

int robotState::setSafetyIndex(double ratio) {
  if (setOneParameter("SETSI", &ratio, DOUBLE_TYPE) == 0) return 0;
  // else
  //   cout << "SETSI : OK" << endr;
  return 1;
}

int robotState::setJvr(double ratio) {
  if (setOneParameter("SETJVR", &ratio, DOUBLE_TYPE) == 0)
    return 0;
  else
    // cout << "SETJVR : OK" << endr;
  return 1;
}

int robotState::setJvs(double ratio) {
  if (setOneParameter("SETJVS", &ratio, DOUBLE_TYPE) == 0)
    return 0;
  // else
  //   cout << "SETJVS : OK" << endr;
  return 1;
}

int robotState::setSwitchFlag4(int flag)
{
  if (setOneParameter("SETSF4", &flag, INT_TYPE) == 0)
    return 0;
  // else
  //   cout << "SETSF4 : OK " << flag << endr;
  return 1;
}

int robotState::setRelMovAndFrameFlag(int relFlag, int frameFlag) {
  int value[2];
  value[0] = frameFlag;
  value[1] = relFlag;

  if (setMultiParameters("SETFM", value, 2, INT_TYPE) == 0)
    return 0;
  // cout << "SETFM : OK" << relFlag<< frameFlag << endr;
  return 1;
}
int robotState::setCartesianImpCtr(int flag) {
  if (setOneParameter("SETCIC", &flag, INT_TYPE) == 0)
    return 0;
  else
    // cout << "SETCIC : OK" << endr;
  return 1;
}

int robotState::setForcelimitProtect(int flag) {
  if (setOneParameter("SETFCRMF", &flag, INT_TYPE) == 0) {
       cout<< "failed"<< endl;
       return 0;
}
  // cout << "SETFCRMF : OK" << endr;
  return 1;
}

int robotState::setP7E(Vector3d &P7E) {
  double value[3];
  for (int i =0; i< 3; i++) value[i] = P7E[i];
  if (setMultiParameters("SETP7E", value, 3, DOUBLE_TYPE) == 0)
    return 0;
  cout << "SETP7E : OK" << P7E.transpose() << endr;
  return 1;
}

int robotState::setLoadParas(Vector4d &LoadParas) {
  double value[4];
  for (int i =0; i< 4; i++) value[i] = LoadParas[i];
  if (setMultiParameters("SETLP", value, 4, DOUBLE_TYPE) == 0)
    return 0;
  cout << "SETLP : OK" << LoadParas.transpose() << endr;
  return 1;
}

/********************  Virtual wall **************************/
int robotState::setVirtualWallLimit(Vector6d &VirWallLimits) {
  double value[6];
  for (int i =0; i< 6; i++) value[i] = VirWallLimits[i];

  if (setMultiParameters("SETVWL", value, 6, DOUBLE_TYPE) == 0)
    return 0;
  cout << "SETVWL : OK" << VirWallLimits.transpose() << endr;
  return 1;
}
int robotState::setVirtualWallAngle(double angle) {
  if (setOneParameter("SETVWA", &angle, DOUBLE_TYPE) == 0)
    return 0;
  else
    cout << "SETVWA : OK" << endr;
  return 1;
}
int robotState::setVirtualWallEnable(int flag) {
  if (setOneParameter("SETVWE", &flag, INT_TYPE) == 0)
    return 0;
  else
    cout << "SETVWE : OK" << endr;
  return 1;
}

/********************  set pos and joints and torque**************************/
int robotState::setJoints(const Vector6d &joints) {
  double value[6];
  for (int i =0; i< 6; i++) value[i] = joints[i];
  if (setMultiParameters("SETJ", value, 6, DOUBLE_TYPE) == 0)
    return 0;
  // cout << "SETJ : OK" << joints.transpose() << endr;
  return 1;
}
int robotState::setJointsVel(const Vector6d &joints) {
  double value[6];
  for (int i =0; i< 6; i++) value[i] = joints[i];

  if (setMultiParameters("SETJV", value, 6, DOUBLE_TYPE) == 0)
    return 0;
  // cout << "SETJV : OK" << joints.transpose() << endr;
  return 1;
}

int robotState::setTotalVel(double m_vel) {
  if (setPvr(m_vel) == 0) return 0;
  if (setJvr(m_vel) == 0) return 0;
  if (setJvs (m_vel) == 0) return 0;
  return 1;
}

int robotState::setPos(const Vector6d &pos) {
  double value[6];
  int rtn = 0;
  cout << "SETP : rtn" << rtn << endr;
  for (int i =0; i< 6; i++) value[i] = pos[i];
  rtn = setMultiParameters("SETP", value, 6, DOUBLE_TYPE);
  cout << "SETP : rtn" << rtn << endr;

  if (rtn == 0)
    return 0;
  cout << "SETP : OK" << pos.transpose() << endr;
  return 1;
}
int robotState::setEndVel(const Vector6d &vel) {
  double value[6];
  int rtn = 0;
  for (int i =0; i< 6; i++) value[i] = vel[i];
  rtn = setMultiParameters("SETEV", value, 6, DOUBLE_TYPE);
  if (rtn == 0)
    return 0;
  return 1;
}
// 设置笛卡尔空间受力超限标志
int robotState::setForceTouchFlag(int flag) {
  if (setOneParameter("SETFTF", &flag, INT_TYPE) == 0)
    return 0;
  // else
  //   cout << "SETFTF : OK" << endr;
  return 1;
}
// set film
int robotState::setForceTouchThres(const Vector6d &thres) {
  double value[6];
  for (int i =0; i< 6; i++) value[i] = thres[i];

  if (setMultiParameters("SETFORCE", value, 6, DOUBLE_TYPE) == 0)
    return 0;
  cout << "SETFORCE : OK" << thres.transpose() << endr;
  return 1;
}

int robotState::setCarForceFeedForward(const Vector6d &ForceForward) {
  double value[6];
  for (int i =0; i< 6; i++) value[i] = ForceForward[i];
  if (setMultiParameters("SETCFFF", value, 6, DOUBLE_TYPE) == 0)
    return 0;
  cout << "SETCFFF : OK" << ForceForward.transpose() << endr;
  return 1;
}
//设置笛卡尔空间刚度
int robotState::setKX(const Vector6d &kx) {
  double value[6];
  for (int i =0; i< 6; i++) value[i] = kx[i];

  if (setMultiParameters("SETKX", value, 6, DOUBLE_TYPE) == 0)
    return 0;
  cout << "SETKX : OK" << kx.transpose() << endr;
  return 1;
}
// 设置笛卡尔空间zuni
int robotState::setKV(const Vector6d &kv) {
  double value[6];
  for (int i =0; i< 6; i++) value[i] = kv[i];

  if (setMultiParameters("SETKV", value, 6, DOUBLE_TYPE) == 0)
    return 0;
  cout << "SETKV : OK" << kv.transpose() << endr;
  return 1;
}

int robotState::setModeOCCM(double MOCCM) {
  if (setOneParameter("SETMOCCM", &MOCCM, DOUBLE_TYPE) == 0) return 0;
  else
    cout << "SETMOCCM : OK" << endr;
  return 1;
}

int robotState::setROCCM(double rOCCM) {
  if (setOneParameter("SETROCCM", &rOCCM, DOUBLE_TYPE) == 0) return 0;
  else
    cout << "SETROCCM : OK" << endr;
  return 1;
}

int robotState::setSelectOPos(double select_O_pos) {
  if (setOneParameter("SETSOP", &select_O_pos, DOUBLE_TYPE) == 0) return 0;
  else
    cout << "SETSOP : OK" << endr;
  return 1;
}

int robotState::setSelectDirection(double select_direction) {
  if (setOneParameter("SETSD", &select_direction, DOUBLE_TYPE) == 0) return 0;
  else
    cout << "SETSD : OK" << endr;
  return 1;
}

//  force reset
int robotState::setForceReset(int flag) {
  if (setOneParameter("SETFR", &flag, INT_TYPE) == 0)
    return 0;
  else
    cout << "SETFR : OK" << endr;
  return 1;
}
/***************  middle level API *****************/
int robotState::movJoint(const Vector6d& joints, double Jvr,
			 int relative, int block) {
  //  if not block, be careful with the flag4!
  int jointFlag = 1;
  if (setSwitchFlag4(0) == 0 ) return 0;
  if (setCartesianImpCtr(0) == 0 ) return 0;
  if (setForceTouchFlag(0) == 0 ) return 0;
  if (setForcelimitProtect(0) == 0 ) return 0;

  if (setMode(2) == 0 ) return 0;
  if (setTotalVel(Jvr) == 0 ) return 0;
  if (relative == 1) {
    Vector6d j2, temp;
    if (getJoints(j2) == 0 ) return 0;
    temp = j2 + joints;
    if (setJoints(temp) == 0 ) return 0;
  } else {
    if (setJoints(joints) == 0 ) return 0;
  }
  usleep(1000);
  if (setSwitchFlag4(1) == 0 ) return 0;
  if (block) {
    jointFlag = 0;
    while (!jointFlag) {
      usleep(10000);
      getJointMovStatusFlag(&jointFlag);
    }
  }
  return 1;
}
int robotState::movJointYoumen(const Vector6d& joints, double Jvr,
			 int relative, int block) {
  //  if not block, be careful with the flag4!
  int jointFlag = 1;
  if (setCartesianImpCtr(0) == 0 ) return 0;
  if (setForceTouchFlag(0) == 0 ) return 0;
  if (setForcelimitProtect(0) == 0 ) return 0;

  if (setMode(2) == 0 ) return 0;
  if (setTotalVel(Jvr) == 0 ) return 0;
  if (relative == 1) {
    Vector6d j2, temp;
    if (getJoints(j2) == 0 ) return 0;
    temp = j2 + joints;
    if (setJoints(temp) == 0 ) return 0;
  } else {
    if (setJoints(joints) == 0 ) return 0;
  }
  if (setSwitchFlag4(1) == 0 ) return 0;

  if (block) {
    jointFlag = 0;
    while (!jointFlag) {
      usleep(10000);
      getJointMovStatusFlag(&jointFlag);
      if (jointFlag) cout << "jointFlag = "<< jointFlag << endr;
    }
  }
  return 1;
}

int robotState::movPosAndForceLimit(const Vector6d& pos,
				    double Pvr, Vector6d Forcelimits,
						Vector6d Forcesensor, int frame, int relative) {
  //  frame = 1 means tool frame, relative = 1 means relative movement
  int posF = 0;
  int touchFlag = 0;
  int forceSensorFlag = 0;

  if (setSwitchFlag4(0) == 0 ) return 0;  // close running
  if (setMode(6) == 0)
		return 0;
	usleep(1000);
  if (setTotalVel(Pvr) == 0 ) return 0;
  if (setRelMovAndFrameFlag(relative, frame)  == 0 ) return 0;
  if (setForceTouchFlag(0) == 0 ) return 0;

  if (setCartesianImpCtr(0) == 0 ) return 0;
  if (setForcelimitProtect(1) == 0) return 0;
  if (setForceTouchThres(Forcesensor)  == 0 ) return 0;

  if (setPos(pos) == 0 ) return 0;
  usleep(10000);
  if (setSwitchFlag4(1) == 0 ) return 0;  // running
  if (setForceReset(1) == 0 ) return 0;  // force reset
  if (setForceReset(0) == 0 ) return 0;  // force reset

  while (!touchFlag && !posF && !forceSensorFlag) {
    usleep(1000);
    getPosMovStatusFlag(&posF);
    if(posF) cout << "pos succeeds" << endr;

    getForceTouchFlag(&touchFlag);
    if(touchFlag) cout << "force exceeds" << endr;
  }
  return 1;
}

int robotState::movJointAndForceLimit(const Vector6d& joints,
				    double Jvr, Vector6d Forcelimits,
						Vector6d Forcesensor,
						int frame, int relative) {
  //  frame = 1 means tool frame, relative = 1 means relative movement
  int jointF = 0;
  int touchFlag = 0;
  int forceSensorFlag = 0;

  if (setSwitchFlag4(0) == 0 ) return 0;  // close running
  if (setMode(2) == 0 ) return 0;
	usleep(100000);
  if (setTotalVel(Jvr) == 0 ) return 0;
  if (setRelMovAndFrameFlag(relative, frame)  == 0 ) return 0;
  if (setForceTouchFlag(0) == 0 ) return 0;

  if (setCartesianImpCtr(0) == 0 ) return 0;
  if (setForcelimitProtect(1) == 0) return 0;
  if (setForceTouchThres(Forcesensor)  == 0 ) return 0;

  if (setJoints(joints) == 0 ) return 0;
  usleep(100000);
  if (setSwitchFlag4(1) == 0 ) return 0;  // running

  if (setForceReset(1) == 0 ) return 0;  // force reset
  if (setForceReset(0) == 0 ) return 0;  // force reset

  //  setInitSensorForce();

  while (!touchFlag && !jointF && !forceSensorFlag) {
    usleep(1000);
    getJointMovStatusFlag(&jointF);
    if (jointF) cout << "joint succeeds" << endr;
    getForceTouchFlag(&touchFlag);
    if (touchFlag) cout << "force exceeds" << endr;
  }
  return 1;
}

int robotState::movJointAndImpCtrl(const Vector6d& joints,
																	 double Pvr,Vector6d KX,
																	 Vector6d KV,
																	 Vector6d forwardforce,
																	 int frame, int relative) {
  // frame = 1 means tool frame, relative = 1 means relative movement
  Vector6d expectedJoints;
  expectedJoints << 0, 0, 0, 0, 0, 0, 0;
  int jointF = 0;
  Vector6d act_joints;
  act_joints << 0, 0, 0, 0, 0, 0, 0;
  double tmp_joint6 = 0;
  Vector6d zeroForce;
  zeroForce << 0, 0, 0, 0, 0, 0;
  int impSF4;
  if (getJoints(act_joints) == 0) return 0;
  expectedJoints = joints + act_joints;

  getSwitchFlag4(&impSF4);
  cout << "imp1 SF4 = " << impSF4 << endr;

  if (setSwitchFlag4(0) == 0 ) return 0;
  getSwitchFlag4(&impSF4);
  cout << "imp2 SF4 = " << impSF4 << endr;

  //  if (setSwitchFlag4(0) == 0 ) return 0;
  if (setMode(6) == 0) return 0;
  if (setRelMovAndFrameFlag(relative, frame) == 0 ) return 0;
  if (setPvr(Pvr) == 0) return 0;

  if (setForceTouchFlag(0) == 0 ) return 0;
  if (setCartesianImpCtr(1) == 0 ) return 0;
  if (setForcelimitProtect(0) == 0 ) return 0;
  if (setKX (KX) == 0) return 0;
  if (setKV (KV) == 0) return 0;
  int rtn = 0;
  rtn = setCarForceFeedForward(forwardforce);
  cout << "setCFFF rtn " << rtn << endr;
  if (rtn == 0)
    return 0;

  while (!jointF) {
    usleep(5000);
    if (getJoints(act_joints) == 0) return 0;

    tmp_joint6 = abs(expectedJoints[6] - act_joints[6]);
    if (tmp_joint6 < 0.5) jointF = 1;
    if (jointF) cout << "jointFlag = "<< jointF << endr;
  }

  if (setSwitchFlag4(0) == 0 ) return 0;

  if (setForceTouchFlag(0) == 0 ) return 0;
  if (setCartesianImpCtr(0) == 0 ) return 0;
  if (setForcelimitProtect(0) == 0 ) return 0;

  Vector6d tmpKX, tmpKV, tmpforwardforce;
  tmpKX << 0, 0, 0, 0, 0, 0;
  tmpKV << 0, 0, 0, 0, 0, 0;
  tmpforwardforce << 0, 0, 0, 0, 0, 0;

  int rtn1 = 0;
  rtn1 = setCarForceFeedForward(tmpforwardforce);
  cout << "setCFFF rtn1 " << rtn1 << endr;
  if (rtn1 == 0)
    return 0;

  if (setKX (tmpKX) == 0) return 0;
  if (setKV (tmpKV) == 0) return 0;

  cout << "impctrl done." << endr;
  return 1;
}

int robotState::movCircle(const Vector6d& targetPos, double mode_OCCM,
			double radius,
			double select_O_pos, double select_direction,
			double Pvr) {
  //  mode_OCCM = 0 :radius,
  //  mode_OCCM = 1 :circle point
  //  select_O_pos = 0 : circle point under the line
  //  select_O_pos = 1 : circle point above the line
  //  select_direction = 0 : smaller arc
  //  select_direction = 1 : bigger arc
  int circleF = 0;

  if (setSwitchFlag4(0) == 0 ) return 0;  // close running

  if (setMode(4) == 0 ) return 0;
  usleep(10000);
  if (setTotalVel(Pvr) == 0 ) return 0;
  if (setRelMovAndFrameFlag(1, 1)  == 0 ) return 0;
  if (setModeOCCM(mode_OCCM) == 0 ) return 0;
  if (setROCCM(radius) == 0 ) return 0;
  if (setSelectOPos(select_O_pos) == 0 ) return 0;
  if (setSelectDirection(select_direction) == 0 ) return 0;

  if (setCartesianImpCtr(0) == 0 ) return 0;
  if (setForcelimitProtect(0) == 0) return 0;
  if (setPos(targetPos) == 0 ) return 0;
  usleep(10000);
  if (setSwitchFlag4(1) == 0 ) return 0;  // running

  while (!circleF) {
    usleep(1000);
    getCircleMovStatusFlag(&circleF);
    if (circleF) cout << "movCircle succeeds" << endr;
  }
  return 1;
}
/***************  high level API *****************/
int robotState::motionInit() {
  if (setSwitchFlag4(0) == 0 ) return 0;
  cout << "set switch flag to 0."<< endr;

  if (setSafetyIndex(0.2) == 0) return 0;
  if (setPvr(0.06) == 0) return 0;
  if (setJvr(0.07) == 0) return 0;
  if (setJvs (0.07) == 0) return 0;

  if (setRelMovAndFrameFlag(1, 1)  == 0 ) return 0;  // relative and tool frame
  if ( setForceTouchFlag(0) == 0) return 0;

  if (setCartesianImpCtr(0) == 0) return 0;

  Vector6d KX, KV, forwardforce;
  KX << 0, 0, 0, 0, 0, 0;
  KV << 0, 0, 0, 0, 0, 0;
  forwardforce << 0, 0, 0, 0, 0, 0;

  if (setKX (KX) == 0) return 0;
  if (setKV (KV) == 0) return 0;
  int rtn = 0;
  rtn = setCarForceFeedForward(forwardforce);
  cout << "setCFFF rtn " << rtn << endr;
  if (rtn == 0)
    return 0;
  Vector6d pushThres;
  pushThres << 1, 1, 1, 1, 1, 1;
  pushThres = pushThres*150;
  if (setForceTouchThres(pushThres) == 0) return 0;
  if (setForcelimitProtect(1) == 0) return 0;
  return 1;
}

int robotState::motionInit(float safeindex, float Jvr, float Pvr) {
  if (setSwitchFlag4(0) == 0 ) return 0;
  if (setSafetyIndex(safeindex) == 0) return 0;
  if (setPvr(Pvr) == 0) return 0;
  if (setJvr(Jvr) == 0) return 0;
  if (setJvs (Jvr) == 0) return 0;
  if (setRelMovAndFrameFlag(1, 1)  == 0 ) return 0;  // relative and tool frame
  if ( setForceTouchFlag(0) == 0) return 0;
  if (setCartesianImpCtr(0) == 0) return 0;

  return 1;
}
// for compatible with TM program
int robotState::movAbsBase(const Vector6d& point) {
  Vector6d thres;
  Vector6d FS_thres;
  if (setSafetyIndex(0.1) == 0) return 0;
  thres << 1, 1, 1, 1, 1, 1;
  thres = thres * 200;
  FS_thres << 1, 1, 1, 1, 1, 1;
  FS_thres = FS_thres * 200;

  movPosAndForceLimit(point, 0.15, thres, FS_thres, 0, 0);
  if (setSafetyIndex(0.2) == 0) return 0;
  return 1;
}

float robotState::PosuJntVel(Vector6d curJnt, Vector6d targJnt, float Jvl) {
  Vector6d deltaJnt, time, posuTime;
  deltaJnt =(targJnt - curJnt).array().abs();
  time = deltaJnt / Jvl;
  posuTime = time.array() / coeffJv.array();
  double T, psT, posuJvl;
  T = time.maxCoeff();
  psT = posuTime.maxCoeff();

  if (psT < 0.001) return Jvl;
  posuJvl = (Jvl * T / psT);
  return posuJvl;
}

int robotState::movAbsJoint(const Vector6d& joint, float Jvl) {
  updateRobotState();
  float posuJvl = PosuJntVel(curJoint, joint, Jvl);
  movJoint(joint, posuJvl, 0, 1);
  return 1;
}

int robotState::movAbsJoint(const Vector6d& joint) {
  movJoint(joint, 0.3, 0, 1);
  return 1;
}
int robotState::movAbsJointYoumen(const Vector6d& joint, float Jvl) {
  movJoint(joint, Jvl, 0, 1);
  return 1;
}
int robotState::movRelTool(const Vector6d& pos, double Pvr) {
  //  frame = 1 means tool frame, relative = 1 means relative movement
  int posF = 0;
  int touchFlag = 0;
  int forceSensorFlag = 0;

  if (setSwitchFlag4(0) == 0 ) return 0;  // close running
  if (setMode(6) == 0 ) return 0;
  if (setTotalVel(Pvr) == 0 ) return 0;
  if (setRelMovAndFrameFlag(1, 1)  == 0 ) return 0;
  if (setForceTouchFlag(0) == 0 ) return 0;

  if (setCartesianImpCtr(0) == 0 ) return 0;
  if (setForcelimitProtect(0) == 0) return 0;

  if (setPos(pos) == 0 ) return 0;
  usleep(200000);
  if (setSwitchFlag4(1) == 0 ) return 0;  // running
  while ( !posF ) {
    usleep(1000);
    getPosMovStatusFlag(&posF);
    if (posF) cout << "pos succeeds" << endr;
  }
	if (setSwitchFlag4(0) == 0)
		return 0;
  return 1;
}
int robotState::movRelJoint(const Vector6d& joint) {
  movJoint(joint, 0.3, 1, 1);
  return 1;
}

int robotState::movRelJoint(const Vector6d& joint, float Jvl) {
  movJoint(joint, Jvl, 1, 1);
  return 1;
}

int robotState::pushButton(Vector6d force, Vector6d movPos,
			   Vector6d FS_thres, double pvr) {
  movPosAndForceLimit(movPos, pvr, force, FS_thres, 1, 1);
  Vector6d tmpPos;
  tmpPos << 0, 0, -0.04, 0, 0, 0;
  movRelTool(tmpPos, 0.06);
  return 1;
}

int robotState::setInitSensorForce() {
  for (int i = 0; i < 6; i++)
    pInitForceSensor[i] = pForceSensor[i];
  return 0;
}

int robotState::getForceSensorFlag(Vector6d forceThreshold) {
  Vector6d deltaForce, flagForce;
  double maxFlag;
  for (int i = 0; i < 6; i++) {
    deltaForce[i] = fabs(pForceSensor[i] - pInitForceSensor[i]);
  }

  flagForce = deltaForce - forceThreshold;
  maxFlag = flagForce.maxCoeff();
  if (maxFlag > 0) {
    cout << "\n\r delta force : " << deltaForce.transpose() << endr;
    return 1;
  }
  return 0;
}
