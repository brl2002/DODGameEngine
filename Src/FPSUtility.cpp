#include "FPSUtility.h"

FPSUtility::FPSUtility()
	:	m_StartTime(0),
		m_Counter(0),
		m_FPS(0)
{
}

FPSUtility::~FPSUtility()
{
}

void FPSUtility::Update( double deltaTime )
{
	m_StartTime += deltaTime;

	m_Counter++;

	if (m_StartTime > 1.0f)
	{
		m_FPS = m_Counter;
		m_Counter = 0;
		m_StartTime = 0;
	}
}