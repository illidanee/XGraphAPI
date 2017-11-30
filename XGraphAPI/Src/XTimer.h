#pragma once

#include <windows.h>

namespace Smile
{
	class XTimer
	{
	public:
		XTimer();
		void Begin();
		void End();
		double GetElapsedTime();

	private:
		LARGE_INTEGER _frequency;
		LARGE_INTEGER _startTime;
		LARGE_INTEGER _endTime;
	};
}