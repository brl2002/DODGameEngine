#include "Render.h"

#include "ResourceManager.h"

#include <stdio.h>
#include <string>

RenderComponent::RenderComponent(char* renderBuffer, char* mapBuffer, int mapBufferWidth, int mapBufferHeight)
	:	m_RenderableBufferArray(renderBuffer),
		m_MapBufferArray(mapBuffer),
		m_MapBufferWidth(mapBufferWidth),
		m_MapBufferHeight(mapBufferHeight),
		m_TotalBufferSize((m_MapBufferWidth + 1) * m_MapBufferHeight + 1)
{}

RenderComponent::~RenderComponent()
{
	delete[] m_MapBufferArray;
	delete[] m_RenderableBufferArray;
}

void RenderComponent::Clear()
{
	// Clear console screen.
	system("cls");

	// Copy map buffer string to rendering buffer string.
	// Analogous to drawing skybox to rendering buffer in 3D rendering techniques.
	memcpy(m_RenderableBufferArray, m_MapBufferArray, m_TotalBufferSize);
}

void RenderComponent::Update(void* renderComponentInst, Entity* entities, int startIndex, int numEntities)
{
	RenderComponent* renderComponent = (RenderComponent*)renderComponentInst;

	int endIndex = startIndex + numEntities;

	for (int i = startIndex; i < endIndex; ++i)
	{
		int index = renderComponent->PositionToArrayIndex(entities[i].position.x, entities[i].position.y);

		renderComponent->m_RenderableBufferArray[index] = entities[i].GetChar();
	}
}

void RenderComponent::Render()
{
	printf(m_RenderableBufferArray);
}