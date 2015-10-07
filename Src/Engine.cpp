#include "Engine.h"

#include <conio.h>
#include "Common.h"
#include "ResourceManager.h"

Engine::Engine()
{
	ResourceManager::getInstance().ReadMapFile("map.txt");

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

	entity = new Entity('P');
	entity->position.x = 10;
	entity->position.y = 5;
}

Engine::~Engine()
{
	delete m_RenderComponent;
	delete m_PhysicsComponent;

	delete entity;
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
				entity->velocity.x = 1;
			}
		}

		if (deltaTime > 0.0167)
		{
			m_RenderComponent->Clear();

			m_RenderComponent->Update(entity);

			m_RenderComponent->Render();

			lastTime = EngineClock::now();
		}
	}
}