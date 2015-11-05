#pragma once

#include "Entity.h"
#include "ITask.h"
#include "MoveToTask.h"
#include "Behavior.h"

class Game
{
	ITask** m_Tasks;

public:
	Game();

	virtual ~Game();

	void BindTask( int taskIndex, Entity* entity );

	static void Update( void* gameInst, Entity** entities, int startIndex, int numEntities, double deltaTime );
};