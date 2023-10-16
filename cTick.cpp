#include "cTick.h"

inline long long cTick::PerformanceFrequency()
{
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	return li.QuadPart;
}

inline long long cTick::PerformanceCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return li.QuadPart;
}



cTick::cTick()
{
	start_ = PerformanceCounter();
	end_ = PerformanceCounter();
	delta_ = (double)((end_ - start_)) / freq_;
}

void cTick::start_tick()
{
	start_ = PerformanceCounter();
}

double cTick::get_tick()
{
	return (double)((PerformanceCounter() - start_)) / freq_;
}

void cTick::reset_tick()
{
	start_ = PerformanceCounter();
	end_ = PerformanceCounter();
	delta_ = (double)((end_ - start_)) / freq_;
}

cTick::~cTick() {};

