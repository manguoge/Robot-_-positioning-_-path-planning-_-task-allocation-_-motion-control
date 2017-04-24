

  //电机1的控制引脚及对应引脚的控制值变量
  int moto1In1=4,moto1In2=5,moto1PwmPin=6;
  float moto1PwmValue=0;
  //电机21的控制引脚及对应引脚的控制值变量
  int moto2In1=7,moto2In2=8,moto2PwmPin=9;
  float moto2PwmValue=0;
   //电机3的控制引脚及对应引脚的控制值变量
  int moto3In1=10,moto3In2=11,moto3PwmPin=12;
  float moto3PwmValue=0;
void setup() 
{
  // 设置串口通讯的速率
  Serial.begin(115200);
  //设置电机的控制引脚为输出模式
  pinMode(moto1In1,OUTPUT);pinMode(moto1In2,OUTPUT);pinMode(moto1PwmPin,OUTPUT);
  pinMode(moto2In1,OUTPUT);pinMode(moto2In2,OUTPUT);pinMode(moto2PwmPin,OUTPUT);
  pinMode(moto3In1,OUTPUT);pinMode(moto3In2,OUTPUT);pinMode(moto3PwmPin,OUTPUT);
  //
  moto1PwmValue=30;moto2PwmValue=30;moto3PwmValue=30;
  clockwiseMoto(moto1In1,moto1In2,moto1PwmPin,moto1PwmValue);
  clockwiseMoto(moto2In1,moto2In2,moto2PwmPin,moto2PwmValue);
  clockwiseMoto(moto3In1,moto3In2,moto3PwmPin,moto3PwmValue);
}
//电机顺时针驱动函数
void clockwiseMoto(int in1,int in2,int pwmPin,float pwmValue)
{
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  analogWrite(pwmPin,pwmValue);
}
void loop()
{
  // put your main code here, to run repeatedly:
  delay(1000);
}
