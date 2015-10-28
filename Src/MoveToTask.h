#pragma once

#include "ITask.h"

class MoveToTask : public ITask
{
public:
	bool DoTask( Enemy* enemy, float deltaTime );
};