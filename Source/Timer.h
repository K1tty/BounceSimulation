#pragma once
#include <chrono>


class CTimer
{
private:
	using TimePoint = std::chrono::high_resolution_clock::time_point;

public:
	CTimer();
	~CTimer();

	void Tick();

	float GetTime() const;
	float GetTickTime() const;
	float GetFPS() const;

private:
	static TimePoint Now();
	static float GetDuration(TimePoint p1, TimePoint p2);

private:
	const TimePoint StartTime;

	TimePoint LastTickTime;
	float TickTime = 0;
};

extern CTimer* GTimer;