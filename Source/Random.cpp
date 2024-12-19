#include "Random.h"
#include <random>


float Random(float Max)
{
  return Random(0, Max);
}

float Random(float Min, float Max)
{
  static std::mt19937 gen(42);
  std::uniform_real_distribution<float> dis(Min, Max);

  return dis(gen);
}