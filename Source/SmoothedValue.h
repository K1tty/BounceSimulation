#pragma once


class CSmoothedValue
{
public:
  void Update(float Value, float SmoothingFactor);
  float Get() const;

private:
  bool Initialized = false;
  float SmoothedValue = 0;
};