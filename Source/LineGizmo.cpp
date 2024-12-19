#include "LineGizmo.h"
#include "Renderer.h"
#include "StrideView.h"


void CLineGizmo::SetStartPoint(const SVector& Point)
{
	Line.P1 = Point;
}

void CLineGizmo::SetEndPoint(const SVector& Point)
{
	Line.P2 = Point;
}

const SLine& CLineGizmo::GetLine() const
{
	return Line;
}

void CLineGizmo::Render()
{
	const TStrideView<SLine> LineView(Line, 1, sizeof(SLine));
	GRenderer->DrawLines(LineView, Colors::Grey);
}
