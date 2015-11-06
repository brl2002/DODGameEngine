#include "Game.h"

Game::Game()
{
	// Initialize task container.
	m_Tasks = new ITask*[1]
	{
		new MoveToTask()
	};
}

Game::~Game()
{
	delete[] m_Tasks;
}

void Game::BindTask( int taskIndex, Entity* entity )
{
	Enemy* enemy = (Enemy*)entity;
	enemy->behavior.tasks.push_back(m_Tasks[taskIndex]);
}

void Game::Update( void* gameInst, Entity** entities, int startIndex, int numEntities, double deltaTime )
{
	Game* game = (Game*)gameInst;

	int maxIndex = startIndex + numEntities;
	for (int i = startIndex; i < maxIndex; ++i)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(entities[i]);

		if ( enemy && (enemy->behavior.tasks[enemy->currentTaskIndex])->DoTask( enemy, deltaTime ) )
		{
			enemy->currentTaskIndex = (enemy->currentTaskIndex + 1) % enemy->behavior.tasks.size();
		}
		else
		{

		}
	}
}