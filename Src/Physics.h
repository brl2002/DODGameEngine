#pragma once

#include "Entity.h"

// PhysicsComponent is responsible for moving entities or characters and making sure that
// things are not occupying same space at any given time.
class PhysicsComponent
{
	// Array of bool, where a single bool value represents whether or not
	// an object, whether movable character or static entity, is occupying that space.
	bool* m_SpaceBuffer;

	// Array of chars to store map data
	//char* m_MapBuffer;

	// Block chars that determine whether an entity can move to a particular space
	char* m_BlockChars;

	// Size of block chars buffer
	int m_BlockCharSize;

	// Width of space buffer, width of map buffer would be m_Width + 1
	int m_Width;

	// Height of both space and map buffer
	int m_Height;

	// Total buffer size of the map buffer
	int m_TotalBufferSize;

	// Total buffer size of the space buffer
	int m_TotalSpaceBufferSize;

public:
	PhysicsComponent(const char* mapBuffer, int width, int height, char* blockChars);

	virtual ~PhysicsComponent();

	// Function to update and compute physics for all provided entities.
	// @param entity Array of entities, @param numEntity How many entities are in the array
	void Update(Entity* entities, int numEntities);

protected:
	void ParallelUpdate(void* param);
};