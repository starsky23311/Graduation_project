/**
 * @file   main.cpp
 * @brief
 * 测试
 * <long description>
 *
 * @author Liu Chenlu, Lin Weiyang
 * @date   2019-05-20
 */
#include "JKRobotControl/main.h"
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
//#include <stringsteam>

using namespace std;

class VSCommandSubscriber : public rclcpp::Node
{
public:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    VSCommandSubscriber()
            : Node("VSCommandSubscriberNode")
    {
        v.resize(6);vsreach = 0;
        subscription_ = this->create_subscription<std_msgs::msg::String>(
                "VisualservoOutput", 10, std::bind(&VSCommandSubscriber::topic_callback, this, std::placeholders::_1));
        publisher_ = this->create_publisher<std_msgs::msg::String>("EncoderDepth", 5);
    }
    vector<float> getEndVelCommand(void)
    {
        return v;
    }
    int getvsreach()
    {
        return this->vsreach;
    }
private:
    void CommandParse(const string & command)
    {
        int step = 0;
        while(1)
        {
            if(step == 0)
            {
                if(*command.begin() == '[' && *(command.end()-1) == ']')
                    step++;
                else
                    step = 0;
            }
            else if(step == 1)
            {
                string s1(command.begin()+1,command.end()-1),s2;
                stringstream ss(s1);int num = 0;
                while(getline(ss, s2, ',')) {
                    if(num < 6) {
                        v[num] = std::atof(s2.c_str());
                    }
                    else
                    {
                        vsreach = std::atoi(s2.c_str());
                    }
                    num++;
                }

                break;
            }
        }
    }
    void topic_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
        CommandParse(msg->data);

    }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;

    vector<float> v;
    int vsreach;
};
class RobotHandle{
public:
    enum ModeType{
        INIT_POSE_MODE,
        JOINT_VEL_MODE,
        VISUAL_SERVO_MODE,
        VISUAL_SERVO_BYENCODER_MODE
    };
    RobotHandle() {
        cout << "press key to select the run mode:\n0:INIT_POSE_MODE"
                "\n1:JOINT_VEL_MODE\n2:VISUAL_SERVO_MODE" << endl;
        CheckKeyEvent();
//        endvel.resize(6);
    }
    //
    ~RobotHandle(){}
    int handle(){
        switch (this->mode)
        {
            case INIT_POSE_MODE: //return to initial pos
            {
                cout<<"ready"<<endl;
                m_robot.setEnable(1);
                cout<<"ready2"<<endl;
                m_robot.setSwitchFlag4(0);


                m_robot.setMode(2);
                Vector6d okpos;
                okpos<<90,20,30,40,-90,0;

                m_robot.setJoints(okpos);
                usleep(10000);
                m_robot.setSwitchFlag4(1);

                break;

            }
            case JOINT_VEL_MODE: //joints vel control
            {
                cout<<"ready"<<endl;
                m_robot.setEnable(1);
                cout<<"ready2"<<endl;
                m_robot.setSwitchFlag4(0);


                m_robot.setMode(1);
                Vector6d okvel;
                okvel<<0,0,0,0,0,5;

                m_robot.setJointsVel(okvel);
                usleep(10000);
                m_robot.setSwitchFlag4(1);

                break;

            }
            case VISUAL_SERVO_MODE:  //EndVel control
            {

//                cout<<endvel.transpose()<<endl;
//                endvel *= 0.5;
                m_robot.setEndVel(endvel);
                usleep(1000);
//                m_robot.setSwitchFlag4(1);

                break;
            }
            case VISUAL_SERVO_BYENCODER_MODE:  //EndVel control
            {
                Vector6d Pos;
                Vector6d end_v_end;
                m_robot.getEndPos(Pos);
                cout<<Pos[2]<<endl;
                if(Pos[2] > 0.23)
                {

                    end_v_end << 1.5,1.5,-3,0,0,0;
                    m_robot.setEndVel(end_v_end);
                }
                else
                {
                    end_v_end << 0,0,0,0,0,0;
                    m_robot.setEndVel(end_v_end);
//                    m_robot.setSwitchFlag4(0);
//                    m_robot.setEnable(0);
                }

                break;
            }
            default:
                break;
        }
        return 1;
    }
    void setEndVel(const vector<float> v)
    {
        endvel<<-v[0],v[1],-v[2],0,0,-v[5];
//        endvel<<1,0,0,0,0,0;
    }

    ModeType getModeType(void)
    {
        return this->mode;
    }

    void setModeType(ModeType setmode)
    {
        this->mode = setmode;
    }
private:
    ModeType mode;
    Vector6d endvel;
    int CheckKeyEvent()
    {
        int ch = getchar();
        if (ch < 0)
        {
            return ch;
            cout<<"Wrong Key"<<endl;
        }
        switch(ch)
        {
            case '0':
                mode = INIT_POSE_MODE;
                break;
            case '1':
                mode = JOINT_VEL_MODE;
                break;
            case '2':
                mode = VISUAL_SERVO_MODE;
                cout<<"ready"<<endl;
                m_robot.setEnable(1);
                cout<<"ready2"<<endl;
                m_robot.setSwitchFlag4(0);
                m_robot.setMode(5);
                usleep(10000);
                m_robot.setSwitchFlag4(1);
                break;
        }

    }
};


int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    cout << "should be : " << argv[0] << "ip port" << endl;
    return 0;
  }
  rclcpp::init(argc, argv);
  std::shared_ptr<VSCommandSubscriber> vscommand =
            std::make_shared<VSCommandSubscriber> ();
  bool tcp_flag = tcp1.setup(argv[1], atoi(argv[2]));
  if (tcp_flag == false)
  {
    cout << "connect failed. Error: No route to host" << endl;
    return 0;
  }
  m_robot.getTcpPointer(&tcp1);
  std::cout<<"Connect successfually!"<<std::endl;
  RobotHandle robot_handle;
  std_msgs::msg::String encoder_depth_msg;

  switch(robot_handle.getModeType())
  {
      case RobotHandle::ModeType::VISUAL_SERVO_MODE :
          while (rclcpp::ok())
          {
              rclcpp::spin_some(vscommand);
              //发送编码器读取高度
              Vector6d encoder_depth_v;
              m_robot.getEndPos(encoder_depth_v);
              stringstream ss;ss<<encoder_depth_v[2];
              encoder_depth_msg.data = ss.str();
              vscommand->publisher_->publish(encoder_depth_msg);

             // robot_handle.setModeType(vscommand->getvsreach()==1 ? RobotHandle::ModeType::VISUAL_SERVO_BYENCODER_MODE : RobotHandle::ModeType::VISUAL_SERVO_MODE);
              robot_handle.setEndVel(vscommand->getEndVelCommand());
              robot_handle.handle();
          }
          break;
      default:
          robot_handle.handle();
  }


  cout << "exit " << endl;
  rclcpp::shutdown();
//  close_keyboard();
  return 0;
}



//int handle(){
//    switch (this->mode)
//    {
//            case 'q': //quit
//            {
//                m_robot.setSwitchFlag4(0);
//                m_robot.setEnable(0);
//                exit(0);
//                break;
//            }

//        case INIT_POSE_MODE: //return to initial pos
//        {
//            cout<<"ready"<<endl;
//            m_robot.setEnable(1);
//            cout<<"ready2"<<endl;
//            m_robot.setSwitchFlag4(0);
//
//
//            m_robot.setMode(2);
//            Vector6d okpos;
//            okpos<<90,20,50,20,-90,0;
//
//            m_robot.setJoints(okpos);
//            usleep(10000);
//            m_robot.setSwitchFlag4(1);
//
//            break;
//
//        }
//        case JOINT_VEL_MODE: //joints vel control
//        {
//            cout<<"ready"<<endl;
//            m_robot.setEnable(1);
//            cout<<"ready2"<<endl;
//            m_robot.setSwitchFlag4(0);
//
//
//            m_robot.setMode(1);
//            Vector6d okvel;
//            okvel<<0,0,0,0,0,5;
//
//            m_robot.setJointsVel(okvel);
//            usleep(10000);
//            m_robot.setSwitchFlag4(1);
//
//            break;



//          m_robot.setSwitchFlag4(0);
////
////
//          m_robot.setMode(1);
//          Vector6d okvel;
////          tcp1.Send("Mov P0");
//          okvel<<0,0,0,0,0,0.3;
//          m_robot.setJointsVel(okvel);
////          tcp1.Send("GETJ");
//          usleep(10000);
//          m_robot.setSwitchFlag4(1);
////    m_robot.getJoints(okvel);
////    std::cout<<okvel<<std::endl;
//          break;
//        }
//        case VISUAL_SERVO_MODE:  //EndVel control
//        {
//          stringstream ss;
//          ss<<"[";
//          for(size_t i = 0; i < 6;i++)
//          {
//              ss<<v[i];
//              if(i !=5)
//                  ss<<",";
//              else
//                  ss<<"]";
//          }
//          cout<<ss.str()<<endl;

//            cout<<"ready"<<endl;
//            m_robot.setEnable(1);
//            cout<<"ready2"<<endl;
//            m_robot.setSwitchFlag4(0);
//            m_robot.setMode(5);
//            Vector6d okvel;
//            okvel<<1,0,0,0,0,1;
//            m_robot.setEndVel(okvel);
//            usleep(10000);
//            m_robot.setSwitchFlag4(1);
//
//
//            break;
//        }
//  case 'w':
//  {
//    m_Vector6d okVel;
//    okVel << 0, 0, 0, 0, 0, 0;
//    m_robot.movEndVel(okVel);
//    m_robot.setSwitchFlag4(0);
//    close_keyboard();
//    exit(0);
//    break;
//  }
//  case 'r':
//  {
//    m_Vector6d okVel;
//    okVel << 0, 0, 0, 0, 0, 0;
//    m_robot.movEndVel(okVel);
//    m_robot.setSwitchFlag4(1);
//    runFlag = 1;
//    break;
//  }
//
//  case '1':
//  {
//    runFlag = 0;
//    m_Vector6d okVel;
//    okVel << 0, 0, 0, 0, 0, 0;
//    m_robot.movEndVel(okVel);
//    m_robot.setSwitchFlag4(0);
//    break;
//  }
//  case '2':
//  {
//    m_robot.movAbsJoint(recordJointPoints[0], 0.3);
//    sleep(3);
//    m_robot.movAbsJoint(recordJointPoints[1], 0.3);
//    sleep(4);
//    runFlag = 1;
//    break;
//  }
//  case '3':
//  {
//    m_robot.movAbsJoint(recordJointPoints[0], 0.3);
//    sleep(3);
//    m_robot.movAbsJoint(recordJointPoints[1], 0.3);
//    sleep(3);
//    runFlag = 0;
//    break;
//  }
//  case '4':
//  {
//    m_robot.movAbsJoint(recordJointPoints[0], 0.3);
//    sleep(3);
//    runFlag = 0;
//    break;
//  }
//
//  case '0':
//  {
//    m_Vector6d joints;
//    m_robot.getJoints(joints);
//    cout << "joints: \r\n " << joints << endl;
//    break;
//  }
