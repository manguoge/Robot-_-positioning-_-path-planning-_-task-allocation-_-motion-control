//获取里程函数
double getDistance()
{
  double averagePlusNum = (moto1PlusNum + moto2PlusNum) * 0.5;
  double distance = averagePlusNum * distanceConstant * sqrt(3);
  //Serial.print("distance:");
  //Serial.println(distance);
  return distance;
}

//获取AHRS测量的方向
float getDirection()
{
  while (!Serial3.available())
  {
    delay(1);
    //Serial.println("available");
  }
  //Serial.println("getDirection1");
  //lastDirection=currentDirection;
  while (!(Serial3.read() == 165 && Serial3.read() == 90 && Serial3.read() == 18 && Serial3.read() == 161))
  {
    delay(1);
    //Serial.println("getDirection3");
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

