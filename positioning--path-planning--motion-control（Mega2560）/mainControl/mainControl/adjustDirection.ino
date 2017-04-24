//给定初始方向和方向误差阈值，超过阈值进行原地转弯校正方向--函数
void adjustDirection(float initDirect,float currentDirect, float dDirect)
{
  //float currentDirect = getDirection();
  float biasAngle;
  bool rotationDirect;

  if (initDirect >= 360 - dDirect)
  {
    if (currentDirect < initDirect)
    {
      biasAngle = initDirect - currentDirect;
      rotationDirect = 0;
    }
    else if (currentDirect > 360 - dDirect)
    {
      biasAngle = currentDirect - initDirect;
      rotationDirect = 1;
    }
    else
    {
      biasAngle = 360 - initDirect + currentDirect;
      rotationDirect = 1;
    }
  }
  else if (initDirect <= dDirect)
  {
    if (currentDirect > initDirect)
    {
      biasAngle = currentDirect - initDirect;
      rotationDirect = 1;
    }
    else if (currentDirect < dDirect)
    {
      biasAngle = initDirect - currentDirect;
      rotationDirect = 0;
    }
    else
    {
      biasAngle = initDirect + 360 - currentDirect;
      rotationDirect = 0;
    }
  }
  else
  {
    if (currentDirect > initDirect)
    {
      biasAngle = currentDirect - initDirect;
      rotationDirect = 1;
    }
    else
    {
      biasAngle = initDirect - currentDirect;
      rotationDirect = 0;
    }
  }
  if (biasAngle >= dDirect)
  {
    Serial.print("initDirect:");
    Serial.println(initDirect);
    Serial.print("currentDirect:");
    Serial.println(currentDirect);
    Serial.print("biasAngle:");
    Serial.println(biasAngle);
    //校正方向前先把直线行驶的速度将为0
    
    runTurn(biasAngle, rotationDirect);
  }
}
