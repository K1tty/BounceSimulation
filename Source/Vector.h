#pragma once


struct SVector
{
	float X = 0;
	float Y = 0;
};


SVector operator*(const SVector& Vector, float Scalar);
SVector operator*(float Scalar, const SVector& Vector);
SVector operator+(const SVector& Vector1, const SVector& Vector2);
SVector operator-(const SVector& Vector1, const SVector& Vector2);
SVector& operator+=(SVector& Vector1, const SVector& Vector2);

SVector Normalize(const SVector& Vector);
SVector Normal(const SVector& Vector);
SVector Reflect(const SVector& Vector, const SVector& Normal);
float Length(const SVector& Vector);
float Dot(const SVector& V1, const SVector& V2);