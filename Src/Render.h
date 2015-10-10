#pragma once

#include "Entity.h"

// RenderComponent is responsible for rendering array of char to the console screen.
class RenderComponent
{
	// Array of chars that will be rendered to the console screen.
	// This is 1-D array is memory for fast access but should be used as 2-D array in abstract.
	char* m_RenderableBufferArray;

	// Array of chars to store map data
	char* m_MapBufferArray;

	// Width of renderableBufferArray
	int m_Width;

	// Height of renderableBufferArray
	int m_Height;

	// Actual char array buffer size that includes line endings and null terminator.
	int m_TotalBufferSize;

public:
	// RenderComponent ctor with width and height dimension of render and map buffer
	RenderComponent(char* renderBuffer, char* mapBuffer, int width, int height);
	virtual ~RenderComponent();

	inline int GetRenderableBufferWidth() { return m_Width; }
	inline int GetRenderableBufferHeight() { return m_Height; }

	// Clear the console screen and copy map buffer to the render buffer
	void Clear();

	// Function to update rendering components for all entities provided.
	// @param entity Array of entities, @param numEntity How many entities are in the array
	void Update(Entity* entities, int numEntities);

	// Renders render buffer to the console screen
	void Render();

protected:
	inline int PositionToArrayIndex(int x, int y) { return y * (m_Width + 1) + x; }
};