#include "Engine.h"

#include <conio.h>
#include "Common.h"
#include "ResourceManager.h"

Engine::Engine()
{
	ResourceManager::getInstance().ReadMapFile("map.txt");
	m_NavBuffer = ResourceManager::getInstance().AllocateNavBuffer("x");

	m_ThreadPool = new ThreadPool<Entity>();

	// Do all inititialization of components here
	m_RenderComponent = new RenderComponent(
		ResourceManager::getInstance().AllocateMapBuffer(),
		ResourceManager::getInstance().AllocateMapBuffer(),
		ResourceManager::getInstance().GetMapBufferWidth(),
		ResourceManager::getInstance().GetMapBufferHeight());

	m_PhysicsComponent = new PhysicsComponent(
		m_NavBuffer,
		ResourceManager::getInstance().GetMapBufferWidth(),
		ResourceManager::getInstance().GetMapBufferHeight());

	m_Entities = new Entity[4] { 'P', 'X', 'X', 'X' };
	m_Entities[0].position.x = 20;
	m_Entities[0].position.y = 10;
	m_Entities[1].position.x = 3;
	m_Entities[1].position.y = 2;
	m_Entities[2].position.x = 8;
	m_Entities[2].position.y = 2;
	m_Entities[3].position.x = 25;
	m_Entities[3].position.y = 2;
}

Engine::~Engine()
{
	delete m_ThreadPool;

	delete m_RenderComponent;
	delete m_PhysicsComponent;

	delete[] m_Entities;

	delete[] m_NavBuffer;
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
				//m_Entities[0].velocity.x = 1;
				g_IsRunning = false;
			}
		}

		if (deltaTime > 0.0167)
		{
			m_RenderComponent->Clear();

			//RenderComponent::Update(m_RenderComponent, m_Entities, 0, 2);
			//RenderComponent::Update(m_RenderComponent, m_Entities, 2, 2);
			m_ThreadPool->AddNewJob(JobDesc<Entity>(m_RenderComponent, &RenderComponent::Update, m_Entities, 0, 2));
			m_ThreadPool->AddNewJob(JobDesc<Entity>(m_RenderComponent, &RenderComponent::Update, m_Entities, 2, 2));
			m_ThreadPool->Wait();

			m_RenderComponent->Render();

			lastTime = EngineClock::now();
		}
	}
}

void Engine::Stop()
{
	g_IsRunning = false;
}