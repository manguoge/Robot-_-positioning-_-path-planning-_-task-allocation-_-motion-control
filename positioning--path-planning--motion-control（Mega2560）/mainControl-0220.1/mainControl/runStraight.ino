//直线行驶给定距离函数
//直线行驶时通过方向校正使小车保持航向
void runStraight(float pwmValue, bool rotationDirection, float targrtDist)
{
  double integralDist = 0, initDist = getDistance(), currentDist = initDist;
  double initDirect = getDirection();
  //直线行驶时，角度偏差阈值，超过阈值校正方向
  float dDirect = 20;
  while (integralDist < targrtDist)
  {
    Serial.println("runStraight--Distance:");
    Serial.println(integralDist);
    pwmValue=locationPID(integralDist,targrtDist);
    runStraight(pwmValue, rotationDirection);
    //校正方向
    float currentDirect = getDirection();
    //adjustDirection(initDirect,currentDirect,dDirect);
    currentDist = getDistance();
    integralDist = currentDist - initDist;
  }
  stop();
}

//直行函数，rotationDirection=0前进或者rotationDirection=1后退
void runStraight(float pwmValue, bool rotationDirection)
{
  moto1PwmValue = pwmValue; moto2PwmValue = pwmValue;
  moto(moto1In1, moto1In2, moto1PwmPin, moto1PwmValue, !rotationDirection);
  moto(moto2In1, moto2In2, moto2PwmPin, moto2PwmValue, rotationDirection);
}


