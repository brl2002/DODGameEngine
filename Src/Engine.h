#pragma once

#include "Physics.h"
#include "AIManager.h"
#include "Game.h"
#include "Entity.h"
#include "ThreadPool.h"
#include "FPSUtility.h"
#include "Profiler.h"
#include "Graphics\Render.h"
#include "Graphics\Input.h"
#include <memory>

#define TIME_PER_FRAME 1.0f / 60.0f

typedef void(*UpdateFunction)(void*, Entity**, int, int, double);

//////////////////////////////////////////////////////////////////////////////////
// Engine class is responsible for managing the entire game system
// and includes render, physics, and AI components.
/////////////////////////////////////////////////////////////////////////////////
class Engine
{
	//ThreadPool<Entity>* m_ThreadPool;

	std::shared_ptr<RenderComponent> m_RenderComponent;

	std::shared_ptr<PhysicsComponent> m_PhysicsComponent;

	std::shared_ptr<AIManager> m_AIManager;

	std::shared_ptr<Game> m_Game;

	std::unique_ptr<FPSUtility> m_FPSUtil;

	std::unique_ptr<Profiler<UpdateFunction>> m_RenderProfiler;

	std::unique_ptr<Profiler<UpdateFunction>> m_PhysicsProfiler;

	std::unique_ptr<Profiler<UpdateFunction>> m_AIProfiler;

	std::unique_ptr<Profiler<UpdateFunction>> m_GameProfiler;

	Entity** m_Entities;

	bool* m_NavBuffer;

	bool m_IsInitializationSuccessful;

	bool m_IsRunning;

	LPCWSTR m_ApplicationName;

	HINSTANCE m_hinstance;

	HWND m_hwnd;

public:
	// Engine constructor, initializes all necessary components.
	Engine();

	// Engine destructor.
	virtual ~Engine();
	
	bool Initialize();

	// Function to start running the engine.
	void Run();

	// Stop running engine.
	void ShutdownEngine();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

protected:
	void InitializeWindows(int& screenWidth, int& screenHeight);

	void ProcessInput();

	void Update( double deltaTime );

	void DisplayDebugInfo();
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static Engine* EngineHandle = 0;