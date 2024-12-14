#pragma once
#include "Line.h"


struct SRectangle
{
	float X = 0;
	float Y = 0;
	float Width = 0;
	float Height = 0;

	SLine Top() const;
	SLine Bottom() const;
	SLine Left() const;
	SLine Right() const;
};