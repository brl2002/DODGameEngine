#include "Render.h"

#include "ResourceManager.h"

#include <stdio.h>
#include <string>

RenderComponent::RenderComponent(char* renderBuffer, char* mapBuffer, int width, int height)
	: m_RenderableBufferArray(renderBuffer), m_MapBufferArray(mapBuffer),
	m_Width(width), m_Height(height), m_TotalBufferSize((m_Width + 1) * m_Height + 1)
{}

RenderComponent::~RenderComponent()
{
	delete[] m_MapBufferArray;
	delete[] m_RenderableBufferArray;
}

void RenderComponent::Clear()
{
	system("cls");

	memcpy(m_RenderableBufferArray, m_MapBufferArray, m_TotalBufferSize);
}

void RenderComponent::Update(Entity* entities, int numEntities)
{
	/*int index = entities->position.y * (m_Width + 1) + entities->position.x;

	m_RenderableBufferArray[index] = entities->GetChar();*/
}

void RenderComponent::Render()
{
	printf(m_RenderableBufferArray);
}