#include "MoveToTask.h"
#include "Math.h"

bool MoveToTask::DoTask( Enemy* enemy, double deltaTime )
{
	if (enemy->path.size() == 0) return true;

	int currentPathIndex = enemy->currentPathIndex;

	// Get chunk index of current segment of path.
	int chunkIndex = enemy->path[currentPathIndex]->GetIndex();

	// Get x and y position of current segment based on its chunk index.
	Vector2D currentIndexPosition = Vector2D( ArrayAccessHelper::GetSimpleColumnIndex( chunkIndex ),
													ArrayAccessHelper::GetSimpleRowIndex( chunkIndex ) );

	//Check if we are close to current position enemy moving towards.
	if (Math::Distance( enemy->position, currentIndexPosition ) < 0.3f)
	{
		// If path index is greater (is not less) than the last index.
		if ( !(currentPathIndex < enemy->path.size() - 1) )
		{
			// Then set the path index back to zero and notify that we are complete with the task.
			enemy->currentPathIndex = 0;
			return true;
		}

		enemy->currentPathIndex = currentPathIndex + 1;

		// Get index of next segment in the path.
		chunkIndex = enemy->path[currentPathIndex]->GetIndex();

		// Set the x and y position based on enemy's current target segment's chunk index.
		currentIndexPosition.x = ArrayAccessHelper::GetSimpleColumnIndex( chunkIndex );
		currentIndexPosition.y = ArrayAccessHelper::GetSimpleRowIndex( chunkIndex );
	}

	// Get displacement vector from enemy's current position to target position.
	Vector2D displacement = Vector2D( currentIndexPosition.x - enemy->position.x,
										currentIndexPosition.y - enemy->position.y );

	// Normalize the displacement value.
	displacement = Math::Normalize( displacement );

	// Set enemy's velocity based on normalized vector displacement times the speed.
	float speed = enemy->GetSpeed();
	enemy->velocity.x = displacement.x * speed;
	enemy->velocity.y = displacement.y * speed;

	return false;
}