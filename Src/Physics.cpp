#include "Physics.h"

#include <string.h>
#include "Common.h"

PhysicsComponent::PhysicsComponent(bool* navBuffer, int mapBufferWidth, int mapBufferHeight)
	:	m_NavBuffer(navBuffer),
		m_MapBufferWidth(mapBufferWidth),
		m_MapBufferHeight(mapBufferHeight),
		m_TotalBufferSize((m_MapBufferWidth + 1) * m_MapBufferHeight + 1),
		m_TotalSpaceBufferSize(m_MapBufferWidth * m_MapBufferHeight)
{}

PhysicsComponent::~PhysicsComponent()
{

}

void PhysicsComponent::Update(Entity* entities, int numEntities)
{

}