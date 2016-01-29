#include "Engine.h"

#include <Windows.h>
#include "Common.h"
#include "ResourceManager.h"
#include "Graph.h"
#include "Player.h"
#include "Enemy.h"

Engine::Engine()
	:	m_IsRunning(false),
		m_NumDividend(0)
{
}

// Deconstructor
Engine::~Engine()
{
	// Deallocate any necessary variables for memory management.

	delete[] m_Entities;

	delete[] m_NavBuffer;
}

bool Engine::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Read a map file first.
	result = ResourceManager::getInstance().ReadMapFile("map.txt");
	if (!result)
	{
		return false;
	}

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	int width = ResourceManager::getInstance().GetMapBufferWidth();
	int height = ResourceManager::getInstance().GetMapBufferHeight();
	int segmentWidth = 3;
	int segmentHeight = 3;
	int chunkWidth = width / segmentWidth;
	int chunkHeight = height / segmentHeight;
	int numSegmentPerChunk = segmentWidth * segmentHeight;

	// Preparing ArrayAccessHelper before using its methods.
	ArrayAccessHelper::Setup(numSegmentPerChunk, chunkWidth, width, height, segmentWidth, segmentHeight);

	// Allocate a navigation buffer based on the map read.
	m_NavBuffer = ResourceManager::getInstance().AllocateNavBuffer("x", segmentWidth, segmentHeight);

#if MULTI_THREADED
	m_ThreadPool = std::make_shared<ThreadPool<Entity>>();
	if (!m_ThreadPool)
	{
		return false;
	}

	m_NumDividend = m_NumEntities / m_ThreadPool->GetConcurrencyCount();
#endif

	// Do all inititialization of components here

	// Create the application wrapper object.
	m_RenderComponent = std::make_shared<RenderComponent>();
	if (!m_RenderComponent)
	{
		return false;
	}

	// Initialize the application wrapper object.
	result = m_RenderComponent->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	m_PhysicsComponent = std::make_shared<PhysicsComponent>(
		m_NavBuffer, width, height, segmentWidth, segmentHeight);

	m_AIManager = std::make_shared<AIManager>(m_NavBuffer, width, height);
	m_AIManager->SetupNavigationGraph(segmentWidth, segmentHeight);

	m_Game = std::make_shared<Game>();

	m_FPSUtil = std::make_unique<FPSUtility>();

	m_Entities = new Entity*[m_NumEntities];

	//m_Entities = new Entity*[m_NumEntities]
	//{
	//	new Player('P', 10), new Enemy('X', 5), new Enemy('X', 5), new Enemy('X', 5),
	//		new Enemy('X', 5), new Enemy('X', 5), new Enemy('X', 5), new Enemy('X', 5),
	//		new Enemy('X', 5), new Enemy('X', 5), new Enemy('X', 5), new Enemy('X', 5),
	//		new Enemy('X', 5), new Enemy('X', 5), new Enemy('X', 5), new Enemy('X', 5),
	//		new Enemy('X', 5), new Enemy('X', 5), new Enemy('X', 5), new Enemy('X', 5)
	//};

	m_Entities[0] = new Player('P', 10);
	for (int i = 1; i < m_NumEntities; ++i)
	{
		m_Entities[i] = new Enemy('X', 5);
	}

	m_Entities[0]->position.x = 20;
	m_Entities[0]->position.y = 10;

	m_Entities[1]->position.x = 3;
	m_Entities[1]->position.y = 2;
	m_Entities[2]->position.x = 8;
	m_Entities[2]->position.y = 2;
	m_Entities[3]->position.x = 25;
	m_Entities[3]->position.y = 2;

	for (int i = 4; i < m_NumEntities; ++i)
	{
		m_Entities[i]->position.x = 25;
		m_Entities[i]->position.y = 2;
	}

	for (int i = 1; i < m_NumEntities; ++i)
	{
		Enemy* enemy = (Enemy*)m_Entities[i];
		enemy->SetTarget(m_Entities[0]);
		m_Game->BindTask(0, enemy);
	}

	m_RenderProfiler = std::make_unique<Profiler<UpdateFunction>>();
	m_PhysicsProfiler = std::make_unique<Profiler<UpdateFunction>>();
	m_AIProfiler = std::make_unique<Profiler<UpdateFunction>>();
	m_GameProfiler = std::make_unique<Profiler<UpdateFunction>>();

	return true;
}

void Engine::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;
	
	// Get an external pointer to this object.
	EngineHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_ApplicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_ApplicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_ApplicationName, m_ApplicationName,
		WS_OVERLAPPEDWINDOW,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);
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

void Engine::ShutdownEngine()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_ApplicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	EngineHandle = NULL;
}

LRESULT CALLBACK Engine::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void Engine::Update(double deltaTime)
{
	m_FPSUtil->Update( deltaTime );

#if _DEBUG
	m_AIProfiler->Start();
#endif

#if MULTI_THREADED
	m_ThreadPool->AddNewJob( JobDesc<Entity>( m_AIManager.get(), &AIManager::Update, m_Entities, 1, m_NumDividend - 1, deltaTime ) );
	m_ThreadPool->AddNewJob( JobDesc<Entity>( m_AIManager.get(), &AIManager::Update, m_Entities, m_NumDividend, m_NumDividend, deltaTime ) );
	m_ThreadPool->AddNewJob( JobDesc<Entity>( m_AIManager.get(), &AIManager::Update, m_Entities, m_NumDividend * 2, m_NumDividend, deltaTime ) );
	m_ThreadPool->AddNewJob( JobDesc<Entity>( m_AIManager.get(), &AIManager::Update, m_Entities, m_NumDividend * 3, m_NumDividend, deltaTime ) );
	m_ThreadPool->Wait();
#else
	AIManager::Update( m_AIManager.get(), m_Entities, 1, m_NumEntities - 1, deltaTime );
#endif

#if _DEBUG
	m_AIProfiler->Capture();
#endif

#if _DEBUG
	m_GameProfiler->Start();
#endif

#if MULTI_THREADED
	m_ThreadPool->AddNewJob( JobDesc<Entity>( m_Game.get(), &Game::Update, m_Entities, 0, m_NumDividend, deltaTime ) );
	m_ThreadPool->AddNewJob( JobDesc<Entity>( m_Game.get(), &Game::Update, m_Entities, m_NumDividend, m_NumDividend, deltaTime ) );
	m_ThreadPool->AddNewJob( JobDesc<Entity>( m_Game.get(), &Game::Update, m_Entities, m_NumDividend * 2, m_NumDividend, deltaTime ) );
	m_ThreadPool->AddNewJob( JobDesc<Entity>( m_Game.get(), &Game::Update, m_Entities, m_NumDividend * 3, m_NumDividend, deltaTime ) );
	m_ThreadPool->Wait();
#else
	Game::Update( m_Game.get(), m_Entities, 0, m_NumEntities, deltaTime );
#endif

#if _DEBUG
	m_GameProfiler->Capture();
#endif

#if _DEBUG
	m_PhysicsProfiler->Start();
#endif

#if MULTI_THREADED
	m_ThreadPool->AddNewJob( JobDesc<Entity>( m_PhysicsComponent.get(), &PhysicsComponent::Update, m_Entities, 0, m_NumDividend, deltaTime ) );
	m_ThreadPool->AddNewJob( JobDesc<Entity>( m_PhysicsComponent.get(), &PhysicsComponent::Update, m_Entities, m_NumDividend, m_NumDividend, deltaTime ) );
	m_ThreadPool->AddNewJob( JobDesc<Entity>( m_PhysicsComponent.get(), &PhysicsComponent::Update, m_Entities, m_NumDividend * 2, m_NumDividend, deltaTime ) );
	m_ThreadPool->AddNewJob( JobDesc<Entity>( m_PhysicsComponent.get(), &PhysicsComponent::Update, m_Entities, m_NumDividend * 3, m_NumDividend, deltaTime ) );
	m_ThreadPool->Wait();
#else
	PhysicsComponent::Update( m_PhysicsComponent.get(), m_Entities, 0, m_NumEntities, deltaTime );
#endif

#if _DEBUG
	m_PhysicsProfiler->Capture();
#endif

#if _DEBUG
	m_RenderComponent->UpdateDebugInfo( m_GameProfiler->GetProfiledTime(), m_AIProfiler->GetProfiledTime(), m_PhysicsProfiler->GetProfiledTime() );

	m_RenderProfiler->Start();
#endif
	// Render.
	m_RenderComponent->StartRender();

	m_IsRunning = m_RenderComponent->Update( m_Entities, 0, m_NumEntities, deltaTime );
	if (!m_IsRunning)
	{
		return;
	}

	m_RenderComponent->StartRender2D();

	m_IsRunning = m_RenderComponent->RenderText();
	if (!m_IsRunning)
	{
		return;
	}

	m_RenderComponent->EndRender();

#if _DEBUG
	m_RenderProfiler->Capture();
#endif
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return EngineHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}