//直线行驶位置PID函数
float locationPID(float currentValue, float targetValue)
{
  static float lastBias;
  float bias, pwmValue, integralBias, p = -1.5, i = 0.3, d = 0.1;
  bias = currentValue - targetValue;
  integralBias += bias;
  pwmValue =  p * bias + i * integralBias + d * (bias - lastBias);
  lastBias = bias;
  if(pwmValue>100)
  pwmValue=100;
  else if(pwmValue<10)
  pwmValue=10;
  return pwmValue;
}
//转向较大角度PID函数
float anglePID(float currentValue, float targetValue)
{
  static float lastBias;
  float bias, pwmValue, integralBias, p, i, d;
  if (targetValue >= 80 && targetValue <= 100)
    p = -0.61, i = 0.1, d = 0.2;
  else  
  p = -1, i = 0, d = 0;
  bias = currentValue - targetValue;
  integralBias += bias;
  pwmValue =  p * bias + i * integralBias + d * (bias - lastBias);
  lastBias = bias;
  return pwmValue;
}
//直线行驶时小角度PID函数
float smallAnglePID(float currentValue, float targetValue)
{
  static float lastBias;
  float bias, pwmValue, integralBias, p, i, d;
  p = -1, i = 0, d = 0;
  bias = currentValue - targetValue;
  integralBias += bias;
  pwmValue =  p * bias + i * integralBias + d * (bias - lastBias);
  lastBias = bias;
  return pwmValue;
}
