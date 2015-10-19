#pragma once

#include "Render.h"
#include "Physics.h"
#include "Entity.h"
#include "ThreadPool.h"

// Engine class is responsible for managing the entire game system
// and includes render, physics, and AI components.
class Engine
{
	//ThreadPool<Entity>* m_ThreadPool;

	RenderComponent* m_RenderComponent;

	PhysicsComponent* m_PhysicsComponent;

	Entity* m_Entities;

	bool* m_NavBuffer;

public:
	// Engine ctor, initializes all necessary components.
	Engine();
	virtual ~Engine();

	// Function to start running the engine.
	void Run();

	void Stop();

protected:

};