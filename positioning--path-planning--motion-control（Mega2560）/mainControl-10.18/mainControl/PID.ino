//直线行驶位置PID函数
float locationPID(float currentValue, float targetValue)
{
  static float lastBias;
  float bias, pwmValue, integralBias, p = -0.61, i = 0.1, d = 0.2;
  bias = currentValue - targetValue;
  integralBias += bias;
  pwmValue =  p * bias + i * integralBias + d * (bias - lastBias);
  lastBias = bias;
  return pwmValue;
}
//转向角度PID函数
float anglePID(float currentValue, float targetValue)
{
  static float lastBias;
  float bias, pwmValue, integralBias, p, i, d;
  if (targetValue >= 80 && targetValue <= 100)
    p = -0.61, i = 0.1, d = 0.2;
  else if (targetValue <= 20)
    p = -20, i = 0, d = 0;
  else  p = -10, i = 0, d = 0;
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
  if (targetValue >= 80 && targetValue <= 100)
    p = -0.61, i = 0.1, d = 0.2;
  else if (targetValue <= 20)
    p = -20, i = 0, d = 0;
  else  p = -10, i = 0, d = 0;
  bias = currentValue - targetValue;
  integralBias += bias;
  pwmValue =  p * bias + i * integralBias + d * (bias - lastBias);
  lastBias = bias;
  return pwmValue;
}
