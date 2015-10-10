#include "Physics.h"

#include <string.h>
#include "Common.h"

PhysicsComponent::PhysicsComponent(bool* navBuffer, int width, int height)
	: m_NavBuffer(navBuffer), m_Width(width), m_Height(height),
	m_TotalBufferSize((m_Width + 1) * m_Height + 1), m_TotalSpaceBufferSize(m_Width * m_Height)
{}

PhysicsComponent::~PhysicsComponent()
{
	delete[] m_NavBuffer;
}

void PhysicsComponent::Update(Entity* entities, int numEntities)
{
	
}