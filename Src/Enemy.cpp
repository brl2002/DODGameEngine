#include "Enemy.h"
#include "Common.h"

// Constructor
Enemy::Enemy( char renderCharacter, float speed )
	:	Entity(renderCharacter),
		currentPathIndex(0),
		currentTaskIndex(0),
		m_ShouldFindPath(true),
		m_Speed(speed)
{}

// Desctructor
Enemy::~Enemy()
{}

void Enemy::Update( float deltaTime )
{
	//if (path.size() == 0) return;

	//// Get chunk index of current segment of path.
	//int chunkIndex = path[m_CurrentPathIndex]->GetIndex();

	//// Get x and y position of current segment based on its chunk index.
	//Vector2D targetPosition = Vector2D( ArrayAccessHelper::GetSimpleColumnIndex( chunkIndex ),
	//										ArrayAccessHelper::GetSimpleRowIndex( chunkIndex ) );

	////Check if we are close to enemy's target position, and if it is clamp the position value to it.
	//if (Math::Distance( position, targetPosition ) < 0.1)
	//{
	//	// If current path index is less than last index of path then increment it.
	//	if (m_CurrentPathIndex < (path.size() - 1)) ++m_CurrentPathIndex;

	//	// Get index of next segment in the path.
	//	chunkIndex = path[m_CurrentPathIndex]->GetIndex();

	//	// Set the x and y position based on enemy's current target segment's chunk index.
	//	targetPosition.x = ArrayAccessHelper::GetSimpleColumnIndex( chunkIndex );
	//	targetPosition.y = ArrayAccessHelper::GetSimpleRowIndex( chunkIndex );

	//	// Get displacement vector from enemy's current position to target position.
	//	Vector2D displacement = Vector2D( targetPosition.x - position.x, targetPosition.y - position.y );

	//	// Normalize the displacement value.
	//	displacement = Math::Normalize( displacement );

	//	// Set enemy's velocity based on normalized vector displacement times the speed.
	//	velocity.x = displacement.x * m_Speed;
	//	velocity.y = displacement.y * m_Speed;
	//}
}