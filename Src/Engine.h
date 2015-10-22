#pragma once

#include "Render.h"
#include "Physics.h"
#include "AIManager.h"
#include "Entity.h"
#include "ThreadPool.h"

// Engine class is responsible for managing the entire game system
// and includes render, physics, and AI components.
class Engine
{
	//ThreadPool<Entity>* m_ThreadPool;

	RenderComponent* m_RenderComponent;

	PhysicsComponent* m_PhysicsComponent;

	AIManager* m_AIManager;

	Entity* m_Entities;

	bool* m_NavBuffer;

	bool m_IsInitializationSuccessful;

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

};