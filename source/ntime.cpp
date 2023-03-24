#include "pch.h"
#include "ntime.h"

#ifdef KAZIN_WINDOWS
#include <windows.h>
#include <Timeapi.h>
#endif

namespace kazin::time
{
	thread_local Value _now = 0;
	thread_local Value _delta = 0;
	thread_local float _delta_float = 0.f;

	void Tick()
	{

#ifdef KAZIN_WINDOWS
		auto now = timeGetTime();
#endif

		_delta = now - _now;
		_delta_float = float(double(now - _now) / 1000);
		_now = now;
	}

	Value Now()
	{
		return _now;
	}

	Value Delta()
	{
		return _delta;
	}

	float DeltaFloat()
	{
		return _delta_float;
	}
}