#include "Intersection.h"


std::optional<SVector> Intersection(const SLine& Line1, const SLine& Line2)
{
	// Line 1 points
	const float X1 = Line1.P1.X;
	const float Y1 = Line1.P1.Y;
	const float X2 = Line1.P2.X;
	const float Y2 = Line1.P2.Y;

	// Line 2 points
	const float X3 = Line2.P1.X;
	const float Y3 = Line2.P1.Y;
	const float X4 = Line2.P2.X;
	const float Y4 = Line2.P2.Y;

	// Calculate the determinants
	const float Denominator = (X1 - X2) * (Y3 - Y4) - (Y1 - Y2) * (X3 - X4);

	// If denominator is zero, lines are parallel or coincident
	if (std::abs(Denominator) < 1e-6)
		return std::nullopt; // No intersection

	// Calculate the intersection point using Cramer's rule
	const float T = ((X1 - X3) * (Y3 - Y4) - (Y1 - Y3) * (X3 - X4)) / Denominator;
	const float U = ((X1 - X3) * (Y1 - Y2) - (Y1 - Y3) * (X1 - X2)) / Denominator;

	// Check if the intersection point is within both line segments
	if (T >= 0 && T <= 1 && U >= 0 && U <= 1)
	{
		const float IntersectX = X1 + T * (X2 - X1);
		const float IntersectY = Y1 + T * (Y2 - Y1);

		return SVector(IntersectX, IntersectY);
	}

	// If T or U is not in [0, 1], the segments do not intersect
	return std::nullopt;
}

bool IsPointInRect(const SVector& Point, const SRectangle& Rect)
{
	return Point.X >= Rect.X && Point.X <= Rect.X + Rect.Width && Point.Y >= Rect.Y && Point.Y <= Rect.Y + Rect.Height;
}

bool IsOverlapping(const SLine& Line, const SRectangle& Rect)
{
	if (IsPointInRect(Line.P1, Rect) || IsPointInRect(Line.P2, Rect))
		return true;

	if (Intersection(Line, Rect.Top()))
		return true;

	if (Intersection(Line, Rect.Bottom()))
		return true;

	if (Intersection(Line, Rect.Left()))
		return true;

	if (Intersection(Line, Rect.Right()))
		return true;

	return false;
}