#pragma once

#include "Common.h"

class FPSUtility
{
	double m_StartTime;
	int m_Counter;
	int m_FPS;

public:
	FPSUtility();
	~FPSUtility();

	void Update( double deltaTime );

	inline int GetFPS() { return m_FPS; }
};