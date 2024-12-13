#pragma once


class CScopedStopwatch
{
public:
	CScopedStopwatch(float& ElapsedTime);
	~CScopedStopwatch();

private:
	float StartTime;
	float& ElapsedTime;
};