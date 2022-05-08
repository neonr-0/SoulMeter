#pragma once

typedef enum {
	end = 0,
	run = 1,
	suspend = 2,
}TIMER_STATUS;

class Timer {
private:
	TIMER_STATUS _status;
	ULONG64 _startTimePoint;
	ULONG64 _suspendTimePoint;
	ULONG64 _suspendedTime;

public:
	Timer();
	~Timer();

	VOID Run();
	VOID Suspend();
	VOID Stop();
	BOOL isRun();

	ULONG64 GetTime();
};