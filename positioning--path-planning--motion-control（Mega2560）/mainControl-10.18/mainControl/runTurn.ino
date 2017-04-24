//左转右转给定角度函数,rotationDirection=0右转,rotationDirection=1左转
//适用80-100度
void runTurn(float angle,bool rotationDirection)
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
    
    Serial.print("rotationDirection:");
    Serial.println(rotationDirection);
    Serial.print("pwmValuePID:");
    Serial.println(pwmValuePID);
    
    if (pwmValuePID > 30)
      pwmValuePID = 30;
    else if (pwmValuePID < 5)
      pwmValuePID = 5;

//    Serial.print("initDirection:");
//    Serial.println(initDirect);
//    Serial.print("currentDirect0:");
//    Serial.println(currentDirect);
//    Serial.print("  pwmValuePID:");
//    Serial.println(pwmValuePID);

    runCycle(pwmValuePID, rotationDirection);
    currentDirect = getDirection();
  }
  stop();
}

//原地转圆函数,rotationDirection=0顺时针,rotationDirection=1逆时针
void runCycle(float pwmValue, bool rotationDirection)
{
  moto(moto1In1, moto1In2, moto1PwmPin, pwmValue, rotationDirection);
  moto(moto2In1, moto2In2, moto2PwmPin, pwmValue, rotationDirection);
  moto(moto3In1, moto3In2, moto3PwmPin, pwmValue, rotationDirection);
}
