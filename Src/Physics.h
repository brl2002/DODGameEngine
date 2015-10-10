#pragma once

#include "Entity.h"

// PhysicsComponent is responsible for moving entities or characters and making sure that
// things are not occupying same space at any given time.
class PhysicsComponent
{
	// Array of bool, where a single bool value represents whether or not
	// an entity can navigate into that space.
	bool* m_NavBuffer;

	// Width of space buffer, width of map buffer would be m_Width + 1
	int m_Width;

	// Height of both space and map buffer
	int m_Height;

	// Total buffer size of the map buffer
	int m_TotalBufferSize;

	// Total buffer size of the space buffer
	int m_TotalSpaceBufferSize;

public:
	PhysicsComponent(bool* navBuffer, int width, int height);

	virtual ~PhysicsComponent();

	// Function to update and compute physics for all provided entities.
	// @param entity Array of entities, @param numEntity How many entities are in the array
	void Update(Entity* entities, int numEntities);
};