void getCoordinate()
{
  float currentDist= getDistance();
  float currentDirect=getDirection();
  x = x + (currentDist - lastDist) * sin(currentDirect / 180.0 * pi);
  y = y + (currentDist - lastDist) * cos(currentDirect / 180.0 * pi);
  float theta = (360 - currentDirect) / 180.0 * pi; //地理坐标系逆时针旋转到小车坐标系的角度
  X = x * cos(theta) + y * sin(theta);
  Y = y * cos(theta) - x * sin(theta);
  lastDist=currentDist;
  Serial.println("Current Coordinate:");
  Serial.print("X:");
  Serial.println(X);
  Serial.print("Y:");
  Serial.println(Y);
  //Serial.print(x);
  //Serial.print("   ");
  //Serial.println(y);
}
void sendXY()
{
  Serial.print("sendXY:X=");
  Serial.println(X);
  Serial2.print(X);
}
