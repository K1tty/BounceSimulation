#include "SmoothedValue.h"
#include <algorithm>

void CSmoothedValue::Update(float Value, float SmoothingFactor)
{
  if (!Initialized)
  {
    SmoothedValue = Value;
    Initialized = true;
  }
  else
  {
    // calculate the exponential moving average
    SmoothingFactor = std::clamp(SmoothingFactor, 0.0f, 1.0f);
    SmoothedValue = SmoothingFactor * Value + (1 - SmoothingFactor) * SmoothedValue;
  }
}

float CSmoothedValue::Get() const
{
	return SmoothedValue;
}