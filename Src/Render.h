#pragma once

#include "Entity.h"
#include <mutex>

////////////////////////////////////////////////////////////////////////////////////////
// RenderComponent is responsible for rendering array of char to the console screen.
///////////////////////////////////////////////////////////////////////////////////////
class RenderComponent
{
	// Array of chars that will be rendered to the console screen.
	// This is 1-D array is memory for fast access but should be used as 2-D array in abstract.
	char* m_RenderableBufferArray;

	// Array of chars to store map data
	char* m_MapBufferArray;

	// Width of renderableBufferArray
	int m_MapBufferWidth;

	// Height of renderableBufferArray
	int m_MapBufferHeight;

	// Actual char array buffer size that includes line endings and null terminator.
	int m_TotalBufferSize;

public:
	// RenderComponent ctor with mapBufferWidth and mapBufferHeight dimension of render and map buffer
	RenderComponent( char* renderBuffer, char* mapBuffer, int mapBufferWidth, int mapBufferHeight );
	
	virtual ~RenderComponent();

	inline int GetRenderableBufferWidth() { return m_MapBufferWidth; }

	inline int GetRenderableBufferHeight() { return m_MapBufferHeight; }

	// Clear the console screen and copy map buffer to the render buffer
	void Clear();

	// Function to update rendering components for all entities provided.
	// @param renderComponentInst instance of RenderComponent that needs update
	// @param entity Array of entities
	// @param startIndex array index to start from
	// @param numEntity How many entities are in the array
	// @param deltaTime time passed since the last frame update
	static void Update( void* renderComponentInst, Entity** entities, int startIndex, int numEntities, double deltaTime );

	// Renders render buffer to the console screen
	void Render();

	void Debug(Entity** entities, int startIndex, int numEntities);

protected:
	inline int PositionToArrayIndex( int x, int y ) { return y * (m_MapBufferWidth + 1) + x; }
};