

//电机1的控制引脚及对应引脚的控制值变量
int moto1In1 = 4, moto1In2 = 5, moto1PwmPin = 6;
float moto1PwmValue = 0;
//电机21的控制引脚及对应引脚的控制值变量
int moto2In1 = 7, moto2In2 = 8, moto2PwmPin = 9;
float moto2PwmValue = 0;
//电机3的控制引脚及对应引脚的控制值变量
int moto3In1 = 10, moto3In2 = 11, moto3PwmPin = 12;
float moto3PwmValue = 0;
//车轮转动方向，0代表顺时针，1代表逆时针
bool rotationDirection = 0;
float lastDirection;
//标志是否是第一次读取AHRS方向，0还未读取，1代表已经读取
bool flag = 0;
void setup()
{
  // 设置串口通讯的速率
  Serial.begin(115200);
  Serial3.begin(115200);
  //设置电机的控制引脚为输出模式
  pinMode(moto1In1, OUTPUT); pinMode(moto1In2, OUTPUT); pinMode(moto1PwmPin, OUTPUT);
  pinMode(moto2In1, OUTPUT); pinMode(moto2In2, OUTPUT); pinMode(moto2PwmPin, OUTPUT);
  pinMode(moto3In1, OUTPUT); pinMode(moto3In2, OUTPUT); pinMode(moto3PwmPin, OUTPUT);
  //调试值
  float pwmValue = 20, angle = 90; rotationDirection = 0;

  //原地转动
  //  runCycle(pwmValue,rotationDirection);
  //直行
  //runStraight(pwmValue, rotationDirection);
  //rotationDirection=0左转,rotationDirection=1右转
  runTurn(angle, pwmValue, rotationDirection);
}
//
float anglePID(float currentValue, float targetValue)
{
  static float lastBias;
  float bias, pwmValue, integralBias, p = 0.5, i = 0, d = 0;
  bias = currentValue - targetValue;
  integralBias += bias;
  pwmValue =abs( p * bias + i * integralBias + d * (bias - lastBias));
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
//左转右转函数,rotationDirection=0右转,rotationDirection=1左转
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
    if ((initDirect >= 0 && initDirect <= 360 - angle))
    {
      integralAngle = currentDirect - initDirect;
    }
    else
    {
      if (currentDirect <= 360)
        integralAngle = currentDirect - initDirect;
      else
        integralAngle = 360 - initDirect + currentDirect;
    }
    //PID
    pwmValuePID =anglePID(integralAngle, angle);
    if(pwmValuePID>50)
    pwmValuePID=30;
    
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
