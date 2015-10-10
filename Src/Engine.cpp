#include "Engine.h"

#include <conio.h>
#include "Common.h"
#include "ResourceManager.h"

Engine::Engine()
{
	ResourceManager::getInstance().ReadMapFile("map.txt");

	//m_ThreadPool = new ThreadPool();

	// Do all inititialization of components here
	m_RenderComponent = new RenderComponent(
		ResourceManager::getInstance().AllocateMapBuffer(),
		ResourceManager::getInstance().AllocateMapBuffer(),
		ResourceManager::getInstance().GetMapBufferWidth(),
		ResourceManager::getInstance().GetMapBufferHeight());

	m_PhysicsComponent = new PhysicsComponent(
		ResourceManager::getInstance().GetMapBuffer(),
		ResourceManager::getInstance().GetMapBufferWidth(),
		ResourceManager::getInstance().GetMapBufferHeight(),
		"x");

	m_Entities = new Entity[2] { 'P', 'X' };
	m_Entities[0].position.x = 20;
	m_Entities[0].position.y = 10;
	m_Entities[1].position.x = 3;
	m_Entities[1].position.y = 2;
}

Engine::~Engine()
{
	//delete m_ThreadPool;

	delete m_RenderComponent;
	delete m_PhysicsComponent;

	delete[] m_Entities;
}

void Engine::Run()
{
	g_IsRunning = true;

	auto currentTime = EngineClock::now();
	auto lastTime = EngineClock::now();

	while (g_IsRunning)
	{
		currentTime = EngineClock::now();

		double deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();

		if (_kbhit())
		{
			int key_code = _getch();

			if (key_code == 100)
			{
				//entity->velocity.x = 1;
			}
		}

		if (deltaTime > 0.0167)
		{
			m_RenderComponent->Clear();

			m_RenderComponent->Update(m_Entities, 2);

			m_RenderComponent->Render();

			lastTime = EngineClock::now();
		}
	}
}