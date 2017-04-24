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



