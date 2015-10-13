#pragma once

#include "Entity.h"

// PhysicsComponent is responsible for moving entities or characters and making sure that
// things are not occupying same space at any given time. Users are expected to allocate
// nav buffer and passed to PhysicsComponent during construction.
class PhysicsComponent
{
	// Array of bool, where a single bool value represents whether or not
	// an entity can navigate into that space.
	bool* m_NavBuffer;

	// Width of space buffer, mapBufferWidth of map buffer would be m_MapBufferWidth + 1
	int m_MapBufferWidth;

	// Height of both space and map buffer
	int m_MapBufferHeight;

	// Total buffer size of the map buffer
	int m_TotalBufferSize;

	// Total buffer size of the space buffer
	int m_TotalSpaceBufferSize;

public:
	// PhysicsComponent ctor, pass nav buffer allocated externally along with mapBufferWidth and mapBufferHeight of
	// map buffer.
	PhysicsComponent(bool* navBuffer, int mapBufferWidth, int mapBufferHeight);

	virtual ~PhysicsComponent();

	// Function to update and compute physics for all provided entities.
	// @param entity Array of entities, @param numEntity How many entities are in the array
	void Update(Entity* entities, int numEntities);
};