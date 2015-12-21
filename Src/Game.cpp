#include "Game.h"
#include "Graphics\Input.h"
#include "Player.h"
#include "Enemy.h"

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
		Player* player = dynamic_cast<Player*>(entities[i]);

		if (enemy)
		{
			if ( (enemy->behavior.tasks[enemy->currentTaskIndex])->DoTask( enemy, deltaTime ) )
			{
				enemy->currentTaskIndex = (enemy->currentTaskIndex + 1) % enemy->behavior.tasks.size();
			}

			enemy->Update( deltaTime );
		}
		else if (player)
		{
			//float horizontalSpeed = 0;
			//float verticalSpeed = 0;

			//if (Input::InputDown(KeyCode::KEY_W))
			//{
			//	verticalSpeed = -1;
			//}
			//else if (Input::InputDown(KeyCode::KEY_S))
			//{
			//	verticalSpeed = 1;
			//}

			//if (Input::InputDown(KeyCode::KEY_D))
			//{
			//	horizontalSpeed = 1;
			//}
			//else if (Input::InputDown(KeyCode::KEY_A))
			//{
			//	horizontalSpeed = -1;
			//}

			//player->MoveVertically(verticalSpeed);
			//player->MoveHorizontally(horizontalSpeed);
		}
	}
}