#include "Physics.h"

#include <string.h>
#include "Common.h"

PhysicsComponent::PhysicsComponent(const char* mapBuffer, int width, int height, char* blockChars)
	: m_Width(width), m_Height(height),
	m_TotalBufferSize((m_Width + 1) * m_Height + 1), m_TotalSpaceBufferSize(m_Width * m_Height),
	m_BlockChars(blockChars), m_BlockCharSize(strlen(blockChars))
{
	m_SpaceBuffer = new bool[m_TotalSpaceBufferSize];

	for (int r = 0; r < m_Height; ++r)
	{
		for (int c = 0; c < m_Width; ++c)
		{
			int spaceBufferIndex = r * m_Width + c;
			int mapBufferIndex = r * (m_Width + 1) + c;

			for (int i = 0; i < m_BlockCharSize; ++i)
			{
				m_SpaceBuffer[spaceBufferIndex] = mapBuffer[mapBufferIndex] == m_BlockChars[i] ? true : false;
			}
		}
	}
}

PhysicsComponent::~PhysicsComponent()
{
	delete[] m_SpaceBuffer;
}

void PhysicsComponent::Update(Entity* entities, int numEntities)
{
	
}