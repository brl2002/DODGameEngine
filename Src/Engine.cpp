#include "Engine.h"

#include <conio.h>
#include "Common.h"
#include "ResourceManager.h"
#include "Graph.h"
#include "Player.h"
#include "Enemy.h"

Engine::Engine()
	:	m_IsRunning(false)
{
	// Read a map file first.
	m_IsInitializationSuccessful = ResourceManager::getInstance().ReadMapFile( "map.txt" );

	int width = ResourceManager::getInstance().GetMapBufferWidth();
	int height = ResourceManager::getInstance().GetMapBufferHeight();
	int segmentWidth = 3;
	int segmentHeight = 3;
	int chunkWidth = width / segmentWidth;
	int chunkHeight = height / segmentHeight;
	int numSegmentPerChunk = segmentWidth * segmentHeight;

	// Preparing ArrayAccessHelper before using its methods.
	ArrayAccessHelper::Setup( numSegmentPerChunk, chunkWidth, width, height, segmentWidth, segmentHeight );

	// Allocate a navigation buffer based on the map read.
	m_NavBuffer = ResourceManager::getInstance().AllocateNavBuffer( "x", segmentWidth, segmentHeight );

	//m_ThreadPool = new ThreadPool<Entity>();

	// Do all inititialization of components here
	m_RenderComponent = new RenderComponent(
		ResourceManager::getInstance().AllocateMapBuffer(),
		ResourceManager::getInstance().AllocateMapBuffer(),
		width, height );

	m_PhysicsComponent = new PhysicsComponent(
		m_NavBuffer, width, height, segmentWidth, segmentHeight );

	m_AIManager = new AIManager( m_NavBuffer, width, height );
	m_AIManager->SetupNavigationGraph( segmentWidth, segmentHeight );

	m_Entities = new Entity*[4] { new Player( 'P' ), new Enemy( 'X', 5 ), new Enemy( 'X', 5 ), new Enemy( 'X', 5 ) };

	m_Entities[0]->position.x = 20;
	m_Entities[0]->position.y = 10;

	m_Entities[1]->position.x = 3;
	m_Entities[1]->position.y = 2;
	m_Entities[2]->position.x = 8;
	m_Entities[2]->position.y = 2;
	m_Entities[3]->position.x = 25;
	m_Entities[3]->position.y = 2;

	for (int i = 1; i < 4; ++i)
	{
		Enemy* enemy = (Enemy*)m_Entities[i];
		enemy->SetTarget(m_Entities[0]);
	}
}

// Deconstructor
Engine::~Engine()
{
	// Deallocate any necessary variables for memory management.

	//delete m_ThreadPool;

	delete m_RenderComponent;
	delete m_PhysicsComponent;
	delete m_AIManager;

	delete[] m_Entities;

	delete[] m_NavBuffer;
}

void Engine::Run()
{
	m_IsRunning = true;

	// Declare frame time calculation related variables.
	auto currentTime = EngineClock::now();
	auto lastTime = EngineClock::now();
	float renderTime = 0;

	// Main loop.
	while (m_IsRunning)
	{
		// Frame time calculation.
		currentTime = EngineClock::now();
		float deltaTime = std::chrono::duration<float>( currentTime - lastTime ).count();

		// Processing inputs prior to doing any updates.
		if (_kbhit())
		{
			int key_code = _getch();

			if (key_code == 100)
			{
				m_IsRunning = false;
			}
		}

		if (deltaTime > 0.0001)
		{
			//AIManager::Update(m_AIManager, m_Entities, 1, 3, deltaTime);

			renderTime += deltaTime;

			// Attemp to render at specific frames per second.
			if (renderTime > 0.0167)
			{
				renderTime = 0;

				PhysicsComponent::Update(m_PhysicsComponent, m_Entities, 0, 4, deltaTime);

				// Clear screen first.
				m_RenderComponent->Clear();

				//m_RenderComponent->Debug(m_Entities, 1, 4);

				RenderComponent::Update(m_RenderComponent, m_Entities, 0, 4, renderTime);
				/*m_ThreadPool->AddNewJob(JobDesc<Entity>(m_RenderComponent, &RenderComponent::Update, m_Entities, 0, 2));
				m_ThreadPool->AddNewJob(JobDesc<Entity>(m_RenderComponent, &RenderComponent::Update, m_Entities, 2, 2));
				m_ThreadPool->Wait();*/

				// Render.
				m_RenderComponent->Render();
			}

			// Capture current time to be used for calculating frame time later.
			lastTime = EngineClock::now();
		}
	}
}

void Engine::Stop()
{
	m_IsRunning = false;
}