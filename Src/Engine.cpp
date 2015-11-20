#include "Engine.h"

#include <conio.h>
#include <ctype.h>
#include <Windows.h>
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

	m_AIManager = std::make_shared<AIManager>( m_NavBuffer, width, height );
	m_AIManager->SetupNavigationGraph( segmentWidth, segmentHeight );

	m_Game = std::make_shared<Game>();

	m_FPSUtil = std::make_unique<FPSUtility>();

	m_Entities = new Entity*[4] { new Player( 'P', 10 ), new Enemy( 'X', 5 ), new Enemy( 'X', 5 ), new Enemy( 'X', 5 ) };

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

	m_RenderProfiler = std::make_unique<Profiler<UpdateFunction>>();
	m_PhysicsProfiler = std::make_unique<Profiler<UpdateFunction>>();
	m_AIProfiler = std::make_unique<Profiler<UpdateFunction>>();
	m_GameProfiler = std::make_unique<Profiler<UpdateFunction>>();
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
	LARGE_INTEGER lastTime, currentTime;
	QueryPerformanceCounter(&lastTime);
	QueryPerformanceCounter(&currentTime);
	double deltaTime = TIME_PER_FRAME;

	// Main loop.
	while (m_IsRunning)
	{
		ProcessInput();

		Update( deltaTime );

		// Frame time calculation.
		QueryPerformanceCounter(&currentTime);
		deltaTime = static_cast<double>( currentTime.QuadPart - lastTime.QuadPart ) / DevicePerformanceProfile::Device_Frequency.QuadPart;

		if (deltaTime > 1.0f)
		{
			deltaTime = TIME_PER_FRAME;
		}

		lastTime = currentTime;
	}
}

void Engine::ProcessInput()
{
	Input::Reset();

	// Processing inputs prior to doing any updates.
	if (_kbhit())
	{
		int key_code = toupper(_getch());

		if (key_code == 0x51)
		{
			m_IsRunning = false;
		}

		Input::Update(key_code);
	}
}

void Engine::Update(double deltaTime)
{
	m_FPSUtil->Update( deltaTime );

#if _DEBUG
	m_AIProfiler->Start();
#endif
	AIManager::Update( m_AIManager.get(), m_Entities, 1, 3, deltaTime );
#if _DEBUG
	m_AIProfiler->Capture();
#endif

#if _DEBUG
	m_GameProfiler->Start();
#endif
	Game::Update( m_Game.get(), m_Entities, 0, 4, deltaTime );
#if _DEBUG
	m_GameProfiler->Capture();
#endif

#if _DEBUG
	m_PhysicsProfiler->Start();
#endif
	PhysicsComponent::Update( m_PhysicsComponent.get(), m_Entities, 0, 4, deltaTime );
#if _DEBUG
	m_PhysicsProfiler->Capture();
#endif

	// Clear screen first.
	m_RenderComponent->Clear();

	//m_RenderComponent->Debug(m_Entities, 1, 4);

#if _DEBUG
	m_RenderProfiler->Start();
#endif
	RenderComponent::Update( m_RenderComponent.get(), m_Entities, 0, 4, deltaTime );
#if _DEBUG
	m_RenderProfiler->Capture();
#endif
	/*m_ThreadPool->AddNewJob(JobDesc<Entity>(m_RenderComponent, &RenderComponent::Update, m_Entities, 0, 2));
	m_ThreadPool->AddNewJob(JobDesc<Entity>(m_RenderComponent, &RenderComponent::Update, m_Entities, 2, 2));
	m_ThreadPool->Wait();*/

	// Render.
	m_RenderComponent->Render();

	DisplayDebugInfo();
}

void Engine::DisplayDebugInfo()
{
	printf("\nPress 'q' to exit program...\n\nDebug:\n");
	printf("FPS = %d\n", m_FPSUtil->GetFPS());
	printf("AI Time = %f\n", m_AIProfiler->GetProfiledTime());
	printf("Game Time = %f\n", m_GameProfiler->GetProfiledTime());
	printf("Physics Time = %f\n", m_PhysicsProfiler->GetProfiledTime());
	printf("Render Time = %f\n", m_RenderProfiler->GetProfiledTime());
}

void Engine::Stop()
{
	m_IsRunning = false;
}