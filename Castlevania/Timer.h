#pragma once

#include <Windows.h>

class Timer
{
	int startTime;
	int limitedTime;
	bool isRun;
public:
	Timer() { init(1000); }
	Timer(int limitedTime) { init(limitedTime); }
	void init(int limitedTime)
	{
		this->limitedTime = limitedTime; startTime = 0; isRun = false;
	}

	void start() { startTime = GetTickCount(); isRun = true; }
	void stop() { startTime = 0; isRun = false; }
	bool isTimeUp()
	{
		if (GetTickCount() - startTime >= limitedTime) {
			return true;
		}
		return false;
	}
	bool isRunning()
	{
		return !isTimeUp() && isRun;
	}

	bool isTimeUpAndRunAlr()
	{
		
		return isTimeUp() && isRun;
	}


	int getTimeRunAlr() { return GetTickCount() - startTime; }

	int getStartTime() { return startTime; }
	int getLimitedTime() { return limitedTime; }
	void setLimitedTime(int val) { limitedTime = val; }
	bool runAlready() const { return isRun; }
};

