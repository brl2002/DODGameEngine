#include "Enemy.h"
#include "Common.h"
#include "Math.h"

// Constructor
Enemy::Enemy( char renderCharacter, float speed )
	:	Entity(renderCharacter),
		m_CurrentPathIndex(0),
		m_ShouldFindPath(true),
		m_Speed(speed)
{}

// Desctructor
Enemy::~Enemy()
{}

void Enemy::Update( float deltaTime )
{
	// Get chunk index of current segment of path.
	int chunkIndex = path[m_CurrentPathIndex]->GetIndex();

	// Get x and y position of current segment based on its chunk index.
	int targetXPosition = ArrayAccessHelper::GetSimpleColumnIndex(chunkIndex);
	int targetYPosition = ArrayAccessHelper::GetSimpleRowIndex(chunkIndex);

	//Check if we are close to enemy's target position, and if it is clamp the position value to it.
	//if ()
	{
		// If current path index is less than last index of path then increment it.
		if (m_CurrentPathIndex < (path.size() - 1)) ++m_CurrentPathIndex;

		// Get index of next segment in the path.
		chunkIndex = path[m_CurrentPathIndex]->GetIndex();

		// Set the x and y position based on enemy's current target segment's chunk index.
		targetXPosition = ArrayAccessHelper::GetSimpleColumnIndex(chunkIndex);
		targetYPosition = ArrayAccessHelper::GetSimpleRowIndex(chunkIndex);

		// Get displacement vector from enemy's current position to target position.
		Vector2D displacement = Vector2D( targetXPosition - position.x, targetYPosition - position.y );

		// Normalize the displacement value.
		displacement = Math::Normalize( displacement );

		// Set enemy's velocity based on normalized vector displacement times the speed.
		velocity.x = displacement.x * m_Speed;
		velocity.y = displacement.y * m_Speed;
	}
}