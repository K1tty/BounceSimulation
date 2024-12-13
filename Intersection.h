#pragma once
#include <optional>
#include "Vector.h"
#include "Line.h"
#include "Rectangle.h"


std::optional<SVector> Intersection(const SLine& Line1, const SLine& Line2);
bool IsPointInRect(const SVector& Point, const SRectangle& Rect);
bool IsOverlapping(const SLine& Line, const SRectangle& Rect);