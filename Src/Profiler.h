#pragma once

#include "Common.h"

template<typename Function>
class Profiler
{
	Function m_Functor;

	LARGE_INTEGER m_StartTime;

	double m_DeltaTime;

public:
	Profiler()
		:	m_DeltaTime(0),
			m_StartTime({0})
	{
	}

	inline void Start()
	{
		QueryPerformanceCounter(&m_StartTime);
	}

	inline void Capture()
	{
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);
		m_DeltaTime = static_cast<double>(end.QuadPart - m_StartTime.QuadPart) / DevicePerformanceProfile::Device_Frequency.QuadPart;
	}

	inline double GetProfiledTime()
	{
		return m_DeltaTime;
	}
};