#pragma once

#include "Render.h"
#include "Physics.h"
#include "Input.h"
#include "AIManager.h"
#include "Game.h"
#include "Entity.h"
#include "ThreadPool.h"
#include "FPSUtility.h"
#include <memory>

#define TIME_PER_FRAME 1.0f / 60.0f

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

	Entity** m_Entities;

	bool* m_NavBuffer;

	bool m_IsInitializationSuccessful;

	bool m_IsRunning;

public:
	// Engine constructor, initializes all necessary components.
	Engine();

	// Engine destructor.
	virtual ~Engine();

	// Function to start running the engine.
	void Run();

	// Stop running engine.
	void Stop();

protected:
	void ProcessInput();

	void Update( double deltaTime );
};