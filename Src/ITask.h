#pragma once

#include "Enemy.h"

class Enemy;

///////////////////////////////////////////////////////
// Interface for AI related task.
///////////////////////////////////////////////////////
class ITask
{
public:
	// Performs AI related task and returns a boolean value specifying
	// whether the task is complete or not.
	// True = task is complete, False = task is incomplete.
	virtual bool DoTask( Enemy* enemy, float deltaTime ) = 0;
};