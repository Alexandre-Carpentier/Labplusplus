#pragma once
#include <Windows.h>

class cTick
{
private:

	inline long long PerformanceFrequency();

	inline long long PerformanceCounter();

	long long start_ = PerformanceCounter();
	long long end_ = PerformanceCounter();
	long long delta_;
	long long freq_ = PerformanceFrequency();

public:

	cTick();

	void start_tick();

	double get_tick();

	void reset_tick();

	~cTick();
};

