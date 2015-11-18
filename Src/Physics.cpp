#include "Physics.h"

#include <string.h>
#include "Common.h"
#include "Math.h"

PhysicsComponent::PhysicsComponent( bool* navBuffer, int mapBufferWidth, int mapBufferHeight, int segmentWidth, int segmentHeight )
	:	m_NavBuffer(navBuffer),
		m_MapBufferWidth(mapBufferWidth - 1),
		m_MapBufferHeight(mapBufferHeight - 1),
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

		float x = entity->position.x + entity->velocity.x * deltaTime;
		float y = entity->position.y + entity->velocity.y * deltaTime;

		x = Math::Clamp( x, 0, physicsComp->m_MapBufferWidth );
		y = Math::Clamp( y, 0, physicsComp->m_MapBufferHeight );

		/*int index = ArrayAccessHelper::GetChunkIndex( y * physicsComp->m_MapBufferWidth + x );

		if (physicsComp->m_NavBuffer[index])
		{
			Vector2D normalizedVelocity = Math::Normalize( entity->velocity );

		}*/

		entity->position.x = x;
		entity->position.y = y;
	}
}