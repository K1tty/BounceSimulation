#pragma once
#include "Vector.h"
#include "Line.h"


class CLineGizmo
{
public:
	void SetStartPoint(const SVector& Point);
	void SetEndPoint(const SVector& Point);

	const SLine& GetLine() const;

	void Render();

private:
	SLine Line;
};