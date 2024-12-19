#include "Intersection.h"


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