#pragma once
#include <cmath>


struct SVector
{
	float X = 0;
	float Y = 0;
};


inline SVector operator*(const SVector& Vector, float Scalar)
{
	return
	{
		.X = Vector.X * Scalar,
		.Y = Vector.Y * Scalar
	};
}

inline SVector operator*(float Scalar, const SVector& Vector)
{
	return operator*(Vector, Scalar);
}

inline SVector operator+(const SVector& Vector1, const SVector& Vector2)
{
	return
	{
		.X = Vector1.X + Vector2.X,
		.Y = Vector1.Y + Vector2.Y
	};
}

inline SVector operator-(const SVector& Vector1, const SVector& Vector2)
{
	return
	{
		.X = Vector1.X - Vector2.X,
		.Y = Vector1.Y - Vector2.Y
	};
}

inline SVector& operator+=(SVector& Vector1, const SVector& Vector2)
{
	Vector1.X += Vector2.X;
	Vector1.Y += Vector2.Y;

	return Vector1;
}

inline bool operator==(const SVector& Vector1, const SVector& Vector2)
{
	return Vector1.X == Vector2.X && Vector1.Y == Vector2.Y;
}

inline float LengthSquared(const SVector& Vector)
{
	return Vector.X * Vector.X + Vector.Y * Vector.Y;
}

inline float DistanceSquared(const SVector& Vector1, const SVector& Vector2)
{
	const float Dx = Vector2.X - Vector1.X;
	const float Dy = Vector2.Y - Vector1.Y;
	return Dx * Dx + Dy * Dy;
}

inline float Length(const SVector& Vector)
{
	return std::sqrt(LengthSquared(Vector));
}

inline SVector Normalize(const SVector& Vector)
{
	const float VectorLength = Length(Vector);

	if (VectorLength != 0)
	{
		return
		{
			.X = Vector.X / VectorLength,
			.Y = Vector.Y / VectorLength
		};
	}

	return { Vector.X, Vector.Y };
}

inline SVector Normal(const SVector& Vector)
{
	return { -Vector.Y, Vector.X };
}

inline float Dot(const SVector& Vector1, const SVector& Vector2)
{
	return Vector1.X * Vector2.X + Vector1.Y * Vector2.Y;
}

inline SVector Reflect(const SVector& Vector, const SVector& Normal)
{
	return Vector - 2 * Dot(Vector, Normal) * Normal;
}

namespace std
{
	template<>
	struct hash<SVector>
	{
		size_t operator()(const SVector& Vector) const
		{
			return std::hash<float>()(Vector.X) ^ (std::hash<float>()(Vector.Y) << 1);
		}
	};
}