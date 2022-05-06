#pragma once

typedef enum {
	end = 0,
	run = 1,
	suspend = 2,
}TIMER_STATUS;

class Timer {
private:
	TIMER_STATUS _status;
	timePoint _startTimePoint;
	timePoint _suspendTimePoint;
	FLOAT _suspendedTime;

public:
	Timer();
	~Timer();

	VOID Run();
	VOID Suspend();
	VOID Stop();
	BOOL isRun();

	FLOAT GetTime();
};