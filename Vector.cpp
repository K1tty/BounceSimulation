#include "Vector.h"
#include <cmath>


SVector operator*(const SVector& Vector, float Scalar)
{
	return
	{
		.X = Vector.X * Scalar,
		.Y = Vector.Y * Scalar
	};
}

SVector operator*(float Scalar, const SVector& Vector)
{
  return operator*(Vector, Scalar);
}

SVector operator+(const SVector& Vector1, const SVector& Vector2)
{
  return
  {
    .X = Vector1.X + Vector2.X,
    .Y = Vector1.Y + Vector2.Y
  };
}

SVector operator-(const SVector& Vector1, const SVector& Vector2)
{
  return
  {
    .X = Vector1.X - Vector2.X,
    .Y = Vector1.Y - Vector2.Y
  };
}

SVector& operator+=(SVector& Vector1, const SVector& Vector2)
{
  Vector1.X += Vector2.X;
  Vector1.Y += Vector2.Y;
	
  return Vector1;
}

SVector Normalize(const SVector& Vector)
{
  const float Length = std::sqrt(Vector.X* Vector.X + Vector.Y* Vector.Y);

  if (Length != 0)
  {
    return
    {
      .X = Vector.X / Length,
      .Y = Vector.Y / Length
    };
  }
  
  return { Vector.X, Vector.Y };
}

SVector Normal(const SVector& Vector)
{
  return { -Vector.Y, Vector.X };
}

SVector Reflect(const SVector& Vector, const SVector& Normal)
{
  return Vector - 2 * Dot(Vector, Normal) * Normal;
}

float Dot(const SVector& Vector1, const SVector& Vector2)
{
  return Vector1.X * Vector2.X + Vector1.Y * Vector2.Y;
}