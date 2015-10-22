#include "Engine.h"

#include <conio.h>
#include "Common.h"
#include "ResourceManager.h"
#include "Graph.h"

Engine::Engine()
{
	// Read a map file first.
	m_IsInitializationSuccessful = 
		ResourceManager::getInstance().ReadMapFile("map.txt");

	int width = ResourceManager::getInstance().GetMapBufferWidth();
	int height = ResourceManager::getInstance().GetMapBufferHeight();
	int segmentWidth = 3;
	int segmentHeight = 3;
	int chunkWidth = width / segmentWidth;
	int chunkHeight = height / segmentHeight;
	int numSegmentPerChunk = segmentWidth * segmentHeight;

	// Preparing ArrayAccessHelper before using its methods.
	ArrayAccessHelper::Setup(numSegmentPerChunk, chunkWidth,
		width, height, segmentWidth, segmentHeight);

	// Allocate a navigation buffer based on the map read.
	m_NavBuffer = ResourceManager::getInstance().AllocateNavBuffer("x", segmentWidth, segmentHeight);

	//m_ThreadPool = new ThreadPool<Entity>();

	// Do all inititialization of components here
	m_RenderComponent = new RenderComponent(
		ResourceManager::getInstance().AllocateMapBuffer(),
		ResourceManager::getInstance().AllocateMapBuffer(),
		width, height );

	m_PhysicsComponent = new PhysicsComponent(
		m_NavBuffer, width, height, segmentWidth, segmentHeight );

	m_AIManager = new AIManager( m_NavBuffer, width, height );
	m_AIManager->SetupNavigationGraph(segmentWidth, segmentHeight);

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

// Deconstructor
Engine::~Engine()
{
	// Deallocate any necessary variables for memory management.

	//delete m_ThreadPool;

	delete m_RenderComponent;
	delete m_PhysicsComponent;

	delete[] m_Entities;

	delete[] m_NavBuffer;
}

void Engine::Run()
{
	g_IsRunning = true;

	// Declare frame time calculation related variables.
	auto currentTime = EngineClock::now();
	auto lastTime = EngineClock::now();

	// Main loop.
	while (g_IsRunning)
	{
		// Frame time computation.
		currentTime = EngineClock::now();
		double deltaTime = std::chrono::duration<double>( currentTime - lastTime ).count();

		// Processing inputs prior to doing any updates.
		if (_kbhit())
		{
			int key_code = _getch();

			if (key_code == 100)
			{
				g_IsRunning = false;
			}
		}

		AIManager::Update( m_AIManager, m_Entities, 0, 4 );

		// Attemp to render at specific frames per second.
		if (deltaTime > 0.0167)
		{
			// Clear screen first.
			m_RenderComponent->Clear();

			m_RenderComponent->Debug(m_Entities, 1, 4);

			RenderComponent::Update( m_RenderComponent, m_Entities, 0, 4 );
			/*m_ThreadPool->AddNewJob(JobDesc<Entity>(m_RenderComponent, &RenderComponent::Update, m_Entities, 0, 2));
			m_ThreadPool->AddNewJob(JobDesc<Entity>(m_RenderComponent, &RenderComponent::Update, m_Entities, 2, 2));
			m_ThreadPool->Wait();*/

			// Render.
			m_RenderComponent->Render();

			// Capture current time to be used for calculating frame time later.
			lastTime = EngineClock::now();
		}
	}
}

void Engine::Stop()
{
	g_IsRunning = false;
}