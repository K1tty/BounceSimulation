#include "ScopedStopwatch.h"
#include "Timer.h"


CScopedStopwatch::CScopedStopwatch(float& InElapsedTime)
	: StartTime(GTimer->GetTime())
	, ElapsedTime(InElapsedTime)
{}

CScopedStopwatch::~CScopedStopwatch()
{
	ElapsedTime = GTimer->GetTime() - StartTime;
}