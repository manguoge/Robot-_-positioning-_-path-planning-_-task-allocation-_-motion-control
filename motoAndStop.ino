////停止函数
void stop()
{
  moto(moto1In1, moto1In2, moto1PwmPin, 0, rotationDirection);
  moto(moto2In1, moto2In2, moto2PwmPin, 0, rotationDirection);
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
