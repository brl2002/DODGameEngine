#include "Engine.h"

#include <conio.h>
#include <ctype.h>
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

	m_RenderComponent = std::make_shared<RenderComponent>(
		ResourceManager::getInstance().AllocateMapBuffer(),
		ResourceManager::getInstance().AllocateMapBuffer(),
		width, height);

	/*m_RenderComponent = new RenderComponent(
		ResourceManager::getInstance().AllocateMapBuffer(),
		ResourceManager::getInstance().AllocateMapBuffer(),
		width, height );*/

	m_PhysicsComponent = std::make_shared<PhysicsComponent>(
		m_NavBuffer, width, height, segmentWidth, segmentHeight );

	m_InputComponent = std::make_shared<InputComponent>();

	m_AIManager = std::make_shared<AIManager>( m_NavBuffer, width, height );
	m_AIManager->SetupNavigationGraph( segmentWidth, segmentHeight );

	m_Game = std::make_shared<Game>();

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
		m_Game->BindTask(0, enemy);
	}
}

// Deconstructor
Engine::~Engine()
{
	// Deallocate any necessary variables for memory management.

	//delete m_ThreadPool;

	delete[] m_Entities;

	delete[] m_NavBuffer;
}

void Engine::Run()
{
	m_IsRunning = true;

	// Declare frame time calculation related variables.
	auto lastTime = EngineClock::now();
	double deltaTime = TIME_PER_FRAME;

	// Main loop.
	while (m_IsRunning)
	{
		ProcessInput();

		Update( deltaTime );

		// Frame time calculation.
		auto currentTime = EngineClock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>( currentTime - lastTime ).count() / 1000.0f;

		if (deltaTime > 1.0f)
		{
			deltaTime = TIME_PER_FRAME;
		}

		lastTime = currentTime;
	}
}

void Engine::ProcessInput()
{
	// Processing inputs prior to doing any updates.
	if (_kbhit())
	{
		int key_code = toupper(_getch());

		if (key_code == 0x51)
		{
			m_IsRunning = false;
		}

		m_InputComponent->Update(key_code);
	}
}

void Engine::Update(double deltaTime)
{
	AIManager::Update( m_AIManager.get(), m_Entities, 1, 3, deltaTime );

	Game::Update( m_Game.get(), m_Entities, 0, 4, deltaTime );

	PhysicsComponent::Update( m_PhysicsComponent.get(), m_Entities, 0, 4, deltaTime );

	// Clear screen first.
	m_RenderComponent->Clear();

	//m_RenderComponent->Debug(m_Entities, 1, 4);

	RenderComponent::Update( m_RenderComponent.get(), m_Entities, 0, 4, deltaTime );
	/*m_ThreadPool->AddNewJob(JobDesc<Entity>(m_RenderComponent, &RenderComponent::Update, m_Entities, 0, 2));
	m_ThreadPool->AddNewJob(JobDesc<Entity>(m_RenderComponent, &RenderComponent::Update, m_Entities, 2, 2));
	m_ThreadPool->Wait();*/

	//deltaTime = deltaTime <= 0 ? TIME_PER_FRAME : deltaTime;
	int fps = 1 / deltaTime;
	printf("FPS = %d Time = %f\n", fps, deltaTime);

	// Render.
	m_RenderComponent->Render();
}

void Engine::Stop()
{
	m_IsRunning = false;
}