uint16_t Approximation(float freq)
{
  float a_freq = 25.5041250154;
  float b_freq = -654.4517172261;
  float c_freq = 6202.7460393771;
  float d_freq = -20518.0111012859;
  uint16_t DAC = 0;
  DAC = a_freq * freq * freq * freq + b_freq * freq * freq + c_freq * freq + d_freq;
  return DAC;
}
  