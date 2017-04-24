#define pi 3.1415926
//编码器线数是390，轮子直径啊60mm,所以每个脉冲的走动距离(cm)是:（pi*60/10）/390
#define distanceConstant 0.0483322
#include <MsTimer2.h>
#include<SoftwareSerial.h>
//电机1的控制引脚及对应引脚的控制值变量
int moto1In1 = 4, moto1In2 = 5, moto1PwmPin = 6;
float moto1PwmValue = 0;
//电机2的控制引脚及对应引脚的控制值变量
int moto2In1 = 7, moto2In2 = 8, moto2PwmPin = 9;
float moto2PwmValue = 0;
//电机3的控制引脚及对应引脚的控制值变量
int moto3In1 = 10, moto3In2 = 11, moto3PwmPin = 12;
float moto3PwmValue = 0;
//车轮转动方向，0代表顺时针，1代表逆时针
bool rotationDirection;
//AHRS上一次测量的方向数据值
float lastDirection;
//标志是否是第一次读取AHRS方向，0还未读取，1代表已经读取
bool flag = 0;
//霍尔传感器（测里程）计数脉冲AB相连接引脚
int moto1PlusA = 2, moto1PlusB = 3, moto2PlusA = 21, moto2PlusB = 20, moto3PlusA = 19, moto3PlusB = 18;
//脉冲计数变量
int moto1PlusNum = 0, moto2PlusNum = 0, moto3PlusNum = 0;
void setup()
{
  // 设置串口通讯的速率
  Serial.begin(115200);
  Serial3.begin(115200);
  //设置电机的控制引脚为输出模式
  pinMode(moto1In1, OUTPUT); pinMode(moto1In2, OUTPUT); pinMode(moto1PwmPin, OUTPUT);
  pinMode(moto2In1, OUTPUT); pinMode(moto2In2, OUTPUT); pinMode(moto2PwmPin, OUTPUT);
  pinMode(moto3In1, OUTPUT); pinMode(moto3In2, OUTPUT); pinMode(moto3PwmPin, OUTPUT);
  //设置脉冲引脚为输入模式
  pinMode(moto1PlusA, INPUT); pinMode(moto1PlusB, INPUT);
  pinMode(moto2PlusA, INPUT); pinMode(moto2PlusB, INPUT);
  pinMode(moto3PlusA, INPUT); pinMode(moto3PlusB, INPUT);
  //将各电机A相计数脉冲中断打开，脉冲上升沿进入脉冲计数函数，
  //当转弯的时候要 关掉中断，去除转弯时的脉冲
  attachInterrupt(0, moto1PlusCount, RISING);
  attachInterrupt(2, moto2PlusCount, RISING);
  attachInterrupt(4, moto3PlusCount, RISING);
  //调试值
  float pwmValue = 30, angle = 90, distance = 100; rotationDirection = 0;

  //原地转动
  //  runCycle(pwmValue,rotationDirection);
  //直行
  //runStraight(pwmValue, rotationDirection);
  runStraight(pwmValue, rotationDirection, distance);
  //rotationDirection=0左转,rotationDirection=1右转
  //runTurn(angle, pwmValue, rotationDirection);
}
//里程计算函数
double getDistance()
{
  double averagePlusNum = (moto1PlusNum + moto2PlusNum) * 0.5;
  double distance = averagePlusNum * distanceConstant * sqrt(3);
  Serial.print("distance:");
  Serial.println(distance);
  return distance;
}
//各电机脉冲上升沿中断时脉冲计数函数
void moto1PlusCount()
{
  if (digitalRead(moto1PlusB) == LOW)
    moto1PlusNum++;
  else
    moto1PlusNum--;
  //  Serial.print("moto1PlusNum:");
  //  Serial.println(moto1PlusNum);
  //getDistance();
}
void moto2PlusCount()
{
  if (digitalRead(moto2PlusB) == HIGH)
    moto2PlusNum++;
  else
    moto2PlusNum--;
  //  Serial.print("moto2PlusNum:");
  //  Serial.println(moto2PlusNum);
  //getDistance();
}
void moto3PlusCount()
{
  if (digitalRead(moto3PlusB) == LOW)
    moto3PlusNum++;
  else
    moto3PlusNum--;
  //  Serial.print("moto3PlusNum:");
  //  Serial.println(moto3PlusNum);
  //getDistance();
}
//直线行驶位置PID函数
float locationPID(float currentValue, float targetValue)
{
  static float lastBias;
  float bias, pwmValue, integralBias, p = -0.61, i = 0.1, d = 0.2;
  bias = currentValue - targetValue;
  integralBias += bias;
  pwmValue =  p * bias + i * integralBias + d * (bias - lastBias);
  lastBias = bias;
  return pwmValue;
}
//转向角度PID函数
float anglePID(float currentValue, float targetValue)
{
  static float lastBias;
  float bias, pwmValue, integralBias, p = -0.61, i = 0.1, d = 0.2;
  bias = currentValue - targetValue;
  integralBias += bias;
  pwmValue =  p * bias + i * integralBias + d * (bias - lastBias);
  lastBias = bias;
  return pwmValue;
}
//停止函数
void stop()
{
  moto(moto1In1, moto1In2, moto1PwmPin, 0, rotationDirection);
  moto(moto2In1, moto2In2, moto2PwmPin, 0, rotationDirection);
  moto(moto3In1, moto3In2, moto3PwmPin, 0, rotationDirection);
}
//直线行驶给定距离函数
//直线行驶时通过方向校正使小车保持航向
void runStraight(float pwmValue, bool rotationDirection, float targrtDist)
{
  double integralDist = 0, initDist = getDistance(), currentDist = initDist;
  double initDirect = getDirection(), currentDirect = initDirect, biasAngle = 0;
  ////直线行驶时，角度偏差阈值，超过阈值校正方向
  float dDirect = 5;
  bool rotationDirect;
  while (integralDist < targrtDist)
  {
    runStraight(pwmValue, rotationDirection);
    if (initDirect >= 360 - dDirect && initDirect <=360)
    {
      if (currentDirect < initDirect)
      {
        biasAngle=initDirect-currentDirect;
        rotationDirect=0;
      }
      else
      {
        biasAngle=360-initDirect+currentDirect;
        rotationDirect=1;
      }
    }
    else if (initDirect >= 0 && initDirect <= dDirect)
    {
      if(currentDirect>initDirect)
      {
        biasAngle=currentDirect-initDirect;
        rotationDirect=1;
      }
      else
      {
        biasAngle=initDirect+360- currentDirect;
        rotationDirect=0;
      }
    }
    else
    {
      if(currentDirect>initDirect)
      {
        biasAngle=currentDirect-initDirect;
        rotationDirect=1;
      }
      else
      {
        biasAngle=initDirect-currentDirect;
        rotationDirect=0;
      }
    }
    if(biasAngle>=dDirect)
    {
      runTurn(biasAngle,pwmValue,rotationDirection);
    }
    currentDist = getDistance();
    integralDist = currentDist - initDist;
  }
  stop();
}
//左转右转给定角度函数,rotationDirection=0右转,rotationDirection=1左转
void runTurn(float angle, float pwmValue, bool rotationDirection)
{
  float initDirect = getDirection();
  float currentDirect = initDirect;
  float pwmValuePID, integralAngle = 0; //累计转动的角度
  while (integralAngle <= angle)
    //((initDirect>=0&&initDirect<=360-angle)&&currentDirect-initDirect<=angle)||(initDirect>360-angle&&currentDirect<=initDirect-360+angle)
    //(initDirection<360-angle&&currentDirect0-initDirection<=angle)||(initDirection>=360-angle&&((currentDirect0>=360-angle)||(currentDirect0<=angle&&currentDirect0<=initDirection-360+angle)))
    //abs(currentDirect0-initDirection)<=angle
  {
    //左转和右转的累积转动角度计算条件不一样
    //rotationDirection=0右转,
    if (rotationDirection == 0)
    {
      if ((initDirect >= 0 && initDirect <= 360 - angle))
        integralAngle = currentDirect - initDirect;
      else if (currentDirect <= 360)
        integralAngle = currentDirect - initDirect;
      else
        integralAngle = 360 - initDirect + currentDirect;
    }
    //rotationDirection=1左转
    else
    {
      if ((initDirect <= 360 && initDirect >= angle))
        integralAngle = abs(currentDirect - initDirect);
      else if (currentDirect <= initDirect)
        integralAngle = abs(currentDirect - initDirect);
      else
        integralAngle = initDirect + 360 - currentDirect;
    }
    //rotationDirection=1左转
    //PID
    pwmValuePID = anglePID(integralAngle, angle);
    if (pwmValuePID > 50)
      pwmValuePID = 30;
    else if (pwmValuePID < 10)
      pwmValuePID = 10;

    Serial.print("initDirection:");
    Serial.println(initDirect);
    Serial.print("currentDirect0:");
    Serial.println(currentDirect);
    Serial.print("  pwmValuePID:");
    Serial.println(pwmValuePID);

    runCycle(pwmValuePID, rotationDirection);
    currentDirect = getDirection();
  }
  stop();
}
//直行函数，rotationDirection=0前进或者rotationDirection=1后退
void runStraight(float pwmValue, bool rotationDirection)
{
  moto1PwmValue = pwmValue; moto2PwmValue = pwmValue; moto3PwmValue = 0;
  moto(moto1In1, moto1In2, moto1PwmPin, moto1PwmValue, !rotationDirection);
  moto(moto2In1, moto2In2, moto2PwmPin, moto2PwmValue, rotationDirection);
  moto(moto3In1, moto3In2, moto3PwmPin, moto3PwmValue, rotationDirection);
}
//原地转圆函数,rotationDirection=0顺时针,rotationDirection=1逆时针
void runCycle(float pwmValue, bool rotationDirection)
{
  moto(moto1In1, moto1In2, moto1PwmPin, pwmValue, rotationDirection);
  moto(moto2In1, moto2In2, moto2PwmPin, pwmValue, rotationDirection);
  moto(moto3In1, moto3In2, moto3PwmPin, pwmValue, rotationDirection);
}
//电机驱动函数,rotationDirection=0顺时针驱动,rotationDirection=1逆时针驱动
void moto(int in1, int in2, int pwmPin, float pwmValue, bool rotationDirection)
{
  if (rotationDirection == 0)
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  else
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  analogWrite(pwmPin, pwmValue);
}
//获取AHRS测量的方向
float getDirection()
{
  while (!Serial3.available())
  {
    //Serial.println("getDirection3");
  }
  //Serial.println("getDirection1");
  //lastDirection=currentDirection;
  while (!(Serial3.read() == 165 && Serial3.read() == 90 && Serial3.read() == 18 && Serial3.read() == 161))
  {
    Serial.println("getDirection3");
  }
  float currentDirection = (Serial3.read() * 256 + Serial3.read()) / 10.0;
  if (flag == 0)
  {
    flag = 1;
    lastDirection = currentDirection;
    //Serial.println("getDirection3");
  }
  if (currentDirection > 360 || currentDirection < 0)
  {
    currentDirection = lastDirection;
  }
  //Serial.println(currentDirection);

  return currentDirection;
}

void loop()
{
  //getDirection();
  //Serial.println(getDirection());
  //delay(1000);
}
