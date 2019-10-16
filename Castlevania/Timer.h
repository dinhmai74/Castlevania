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

	void start() { startTime = GetTickCount64(); isRun = true; }
	void stop() { startTime = 0; isRun = false; }
	bool isTimeUp() const {
		if (GetTickCount64() - startTime >= limitedTime) {
			return true;
		}
		return false;
	}
	bool isTimeUp(DWORD delayTime) const {
		if (GetTickCount64() - startTime  >= limitedTime +delayTime) {
			return true;
		}
		return false;
	}
	bool isRunning() const {
		return !isTimeUp() && isRun;
	}

	bool isTimeUpAndRunAlr() const {
		return isTimeUp() && isRun;
	}

	int getTimeRunAlr() const { return GetTickCount64() - startTime; }

	int getStartTime() const { return startTime; }
	int getLimitedTime() const { return limitedTime; }
	void setLimitedTime(int val) { limitedTime = val; }
	bool runAlready() const { return isRun; }
};
