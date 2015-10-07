#pragma once

#include "Render.h"
#include "Physics.h"
#include "Entity.h"

// Engine class is responsible for managing the entire game system
// and includes render, physics, and AI components.
class Engine
{
	RenderComponent* m_RenderComponent;

	PhysicsComponent* m_PhysicsComponent;

	Entity* entity;

public:
	Engine();
	virtual ~Engine();

	void Run();

protected:
};