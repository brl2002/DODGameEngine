#include "Physics.h"

#include <string.h>
#include "Common.h"

PhysicsComponent::PhysicsComponent( bool* navBuffer, int mapBufferWidth, int mapBufferHeight, int segmentWidth, int segmentHeight )
	:	m_NavBuffer(navBuffer),
		m_MapBufferWidth(mapBufferWidth),
		m_MapBufferHeight(mapBufferHeight),
		m_TotalBufferSize((m_MapBufferWidth + 1) * m_MapBufferHeight + 1),
		m_TotalSpaceBufferSize(m_MapBufferWidth * m_MapBufferHeight),
		m_SegmentWidth(segmentWidth),
		m_SegmentHeight(segmentHeight),
		m_ChunkWidth(m_MapBufferWidth / m_SegmentWidth),
		m_ChunkHeight(m_MapBufferHeight / m_SegmentHeight)
{}

PhysicsComponent::~PhysicsComponent()
{

}

void PhysicsComponent::Update( void* physicsComponentInst, Entity** entities, int startIndex, int numEntities, double deltaTime )
{
	PhysicsComponent* physicsComp = (PhysicsComponent*)physicsComponentInst;

	int maxIndex = startIndex + numEntities;
	for (int i = startIndex; i < maxIndex; ++i)
	{
		Entity* entity = entities[i];
		
		entity->position.x += entity->velocity.x * deltaTime;
		entity->position.y += entity->velocity.y * deltaTime;

		//int index = ArrayAccessHelper::GetChunkIndex( y * physicsComp->m_MapBufferWidth + x );

		//if (physicsComp->m_NavBuffer[index])
		//{
		//	entity->position.x = x;
		//	entity->position.y = y;
		//}
	}
}