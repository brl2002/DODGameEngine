#pragma once

#include <vector>
#include "ITask.h"

/////////////////////////////////////////////////////////////////////
// Class responsible for doing AI related tasks. Holds list of tasks
// and processes them in order and in circular fashion.
/////////////////////////////////////////////////////////////////////
class Behavior
{
	std::vector<ITask*> m_Tasks;

	int m_CurrentTaskIndex;

public:
	Behavior();

	virtual ~Behavior();

	// Perform current AI task.
	void DoTask();

	inline void AddTask(ITask* task) { m_Tasks.push_back( task ); }
};