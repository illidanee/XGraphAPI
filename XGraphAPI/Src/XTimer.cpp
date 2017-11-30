#include "XTimer.h"

namespace Smile
{
	/****************************************************************************************************************
	 *    Date    : 2017/11/29 10:06
	 *
	 *    Author  : Smile
	 *    Contact : smile@illidan.org
	 *
	 *    Brief   : high-performance Timer
	 *
	 ****************************************************************************************************************/
	XTimer::XTimer()
	{
		QueryPerformanceFrequency(&_frequency);
		QueryPerformanceCounter(&_startTime);
	}

	void XTimer::Begin()
	{
		QueryPerformanceCounter(&_startTime);
	}

	void XTimer::End()
	{
		QueryPerformanceCounter(&_endTime);
	}

	double XTimer::GetElapsedTime()
	{
		return (_endTime.QuadPart - _startTime.QuadPart) * (1000000.0 / _frequency.QuadPart);
	}
}