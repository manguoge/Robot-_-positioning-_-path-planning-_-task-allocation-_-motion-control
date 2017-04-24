//1.采用定时器试试校正直线行驶的方向误差
//2.转弯脉冲不计数
//3.直线行驶pid
#define pi 3.1415926
//编码器线数是390，轮子直径啊60mm,所以每个脉冲的走动距离(cm)是:（pi*60/10）/390
#define distanceConstant 0.0483322
//#include <MsTimer2.h>
#include<FlexiTimer2.h>
#include <TimerOne.h>
#include<SoftwareSerial.h>

//电机1的控制引脚及对应引脚的控制值变量
int moto1In1 = 4, moto1In2 = 5, moto1PwmPin = 6;
float moto1PwmValue = 0;
//电机2的控制引脚及对应引脚的控制值变量
int moto2In1 = 7, moto2In2 = 8, moto2PwmPin = 9;
float moto2PwmValue = 0;

//车轮转动方向，0代表顺时针，1代表逆时针
bool rotationDirection=0;
//AHRS上一次测量的方向数据值
float lastDirection;
//标志是否是第一次读取AHRS方向，0还未读取，1代表已经读取
bool flag = 0;
//霍尔传感器（测里程）计数脉冲AB相连接引脚
int moto1PlusA = 2, moto1PlusB = 3, moto2PlusA = 21, moto2PlusB = 20;
//脉冲计数变量
int moto1PlusNum = 0, moto2PlusNum = 0;
//地球坐标系下的小车坐标
float x=0,y=0;
//小车坐标系下的小车坐标
float X=0,Y=0;
//记录小车在定时器的上一次中断时的路程
float lastDist=0;

  //调试值
float pwmValue = 30, angle = 90, distance = 100;// rotationDirection = 0;
void setup()
{
  // 设置串口通讯的速率
  Serial.begin(115200);
  Serial3.begin(115200);
  Serial2.begin(115200);
  //设置电机的控制引脚为输出模式
  pinMode(moto1In1, OUTPUT); pinMode(moto1In2, OUTPUT); pinMode(moto1PwmPin, OUTPUT);
  pinMode(moto2In1, OUTPUT); pinMode(moto2In2, OUTPUT); pinMode(moto2PwmPin, OUTPUT);
  //设置脉冲引脚为输入模式
  pinMode(moto1PlusA, INPUT); pinMode(moto1PlusB, INPUT);
  pinMode(moto2PlusA, INPUT); pinMode(moto2PlusB, INPUT);
  //将各电机A相计数脉冲中断打开，脉冲上升沿进入脉冲计数函数，
  //当转弯的时候要 关掉中断，去除转弯时的脉冲
  attachInterrupt(0, moto1PlusCount, RISING);
  attachInterrupt(2, moto2PlusCount, RISING);
  
  float x=0,y=0;//地理坐标系下车位置坐标
  float X=0,Y=0;//小车坐标系下车的坐标
  //根据积分的思想，利用定时器每隔0.01s刷新小车坐标
  FlexiTimer2::set(500, getCoordinate); // 500ms period
  FlexiTimer2::start();
//打开中断，每隔1s向leonardo发送位置坐标
  Timer1.initialize(1000000);//us 
  Timer1.attachInterrupt(sendXY); 
  //原地转动
  //runCycle(pwmValue,rotationDirection);
  //直行
  //runStraight(pwmValue, rotationDirection);
  //runStraight(pwmValue, rotationDirection, distance);
  //rotationDirection=0左转,rotationDirection=1右转
  //runTurn(angle,rotationDirection);
  //
}

void loop()
{
 // runStraight(pwmValue, rotationDirection, distance);
    //getCoordinate();
//  float currentDirection = getDirection();
  //Serial.println("currentDirection:");
//  Serial.println(currentDirection);
  delay(300);
}


