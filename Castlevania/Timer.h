#pragma once

#include <Windows.h>

class Timer
{
	int startTime;
	int limitedTime;
	bool isRun;
public:
	Timer(int limitedTime) { this->limitedTime = limitedTime; startTime = 0; isRun = false; }

	void start() { startTime = GetTickCount(); isRun = true; }
	void stop() { startTime = 0; isRun = false; }
	bool isTimeUp()
	{
		if (GetTickCount() - startTime >= limitedTime) {
			if (runAlready()) isRun = false;
			return true;
		}
		return false;
	}
	bool isRunning()
	{
		return !isTimeUp() && isRun;
	}

	int getStartTime() { return startTime; }
	int getLimitedTime() { return limitedTime; }
	bool runAlready() const { return isRun; }
};

