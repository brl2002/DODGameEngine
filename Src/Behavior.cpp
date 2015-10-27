#include "Behavior.h"

// Constructor
Behavior::Behavior()
	:	m_CurrentTaskIndex(0)
{}

// Destructor
Behavior::~Behavior()
{}

void Behavior::DoTask()
{
	if (!m_Tasks[m_CurrentTaskIndex]->DoTask())
	{
		m_CurrentTaskIndex = (m_CurrentTaskIndex + 1) % m_Tasks.size();
	}
}