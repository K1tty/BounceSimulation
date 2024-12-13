#include "Timer.h"


CTimer* GTimer = nullptr;

CTimer::CTimer()
	: StartTime(Now())
	, LastTickTime(Now())
{
	GTimer = this;
}

CTimer::~CTimer()
{
	GTimer = nullptr;
}

void CTimer::Tick()
{
	const TimePoint CurrentTime = Now();

	TickTime = GetDuration(LastTickTime, CurrentTime);
	LastTickTime = CurrentTime;
}

CTimer::TimePoint CTimer::Now()
{
	return std::chrono::high_resolution_clock::now();
}

float CTimer::GetDuration(TimePoint p1, TimePoint p2)
{
	return std::chrono::duration_cast<std::chrono::microseconds>(p2 - p1).count() / 1'000'000.0f;
}

float CTimer::GetTime() const
{
	return GetDuration(StartTime, Now());
}

float CTimer::GetTickTime() const
{
	return TickTime;
}

float CTimer::GetFPS() const
{
	return 1.0f / TickTime;
}