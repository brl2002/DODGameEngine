#include "AIManager.h"
#include "Common.h"
#include "Enemy.h"
#include <vector>
#include <unordered_map>
#include <queue>
#include <tuple>

// Constructor
AIManager::AIManager( const bool* navBuffer, int mapBufferWidth, int mapBufferHeight )
	:	m_MapBufferWidth(mapBufferWidth),
		m_MapBufferHeight(mapBufferHeight),
		m_MapGraph(new Graph(m_MapBufferWidth * m_MapBufferHeight))
{
	// Use navBuffer to create navigation buffer array organized efficiently for AIManager
	m_NavBuffer = navBuffer;
}

// Desctructor
AIManager::~AIManager()
{
	delete m_MapGraph;
}

void AIManager::Update( void* aiManagerInst, Entity** entities, int startIndex, int numEntities, double deltaTime )
{
	// Cast to AIManager pointer for us to use.
	AIManager* aiManager = (AIManager*)aiManagerInst;

	// Get graph and map buffer width from AIManager instance.
	Graph* graph = aiManager->m_MapGraph;
	int width = aiManager->m_MapBufferWidth;

	// For each entity in the list.
	int maxIndex = startIndex + numEntities;
	for (int i = startIndex; i < maxIndex; ++i)
	{
		// Cast current Entity to Enemy and get its target.
		Enemy* enemy = dynamic_cast<Enemy*>(entities[i]);
		Entity* target = enemy->GetTarget();

		// Get segment where enemy is currently at.
		// Add 0.5 for proper rounding of the floating point values
		int currentIndex = (int)(enemy->position.y + 0.5) * width + (int)(enemy->position.x + 0.5);
		Segment* currentSegment = graph->GetSegment( ArrayAccessHelper::GetChunkIndex( currentIndex ) );

		// Get segment where enemy's target is currently at.
		int targetIndex = (int)(target->position.y + 0.5) * width + (int)(target->position.x + 0.5);
		Segment* targetSegment = graph->GetSegment( ArrayAccessHelper::GetChunkIndex( targetIndex ) );

		// If enemy requires new path and enemy's last target segment is not the segment where target is actually at.
		if (enemy->ShouldFindPath() && enemy->GetLastTargetSegment() != targetSegment)
		{
			// Initialize checked segments and cost for use.
			CheckedSegments checkedSegments;
			Cost cost;

			// Find path for current enemy from current segment it is at to segment enemy's target is at.
			FindPath( currentSegment, targetSegment, checkedSegments, cost );

			// Reconstruct path using list of checked segments and assign to enem's path.
			enemy->path = ReconstructPath( currentSegment, targetSegment, checkedSegments );

			// Set enemy's last target segment to segment that enemy's target is at.
			enemy->SetLastTargetSegment( targetSegment );

			enemy->SetShouldFindPath( false );

			enemy->currentPathIndex = 0;

			//enemy->position.x = (int)enemy->position.x;
			//enemy->position.y = (int)enemy->position.y;
		}
	}
}

void AIManager::SetupNavigationGraph( int segmentWidth, int segmentHeight )
{
	// From chunk index to linear index and use look-up table to find addition value;
	// and once you add that value to the linear index, convert it back to chunk index

	int totalSize = m_MapBufferWidth * m_MapBufferHeight;
	int lastColumnIndex = m_MapBufferWidth - 1;

	for (int chunkIndex = 0; chunkIndex < totalSize; ++chunkIndex)
	{
		auto&& segment = m_MapGraph->GetSegment( chunkIndex );
		segment->SetIndex( chunkIndex );

		int simpleIndex = ArrayAccessHelper::GetSimpleIndex( chunkIndex );

		for (int i = 0; i < 9; ++i)
		{
			if (i != 4)
			{
				int addFactor = ArrayAccessHelper::GetAddFactor( i );
				int cost = ArrayAccessHelper::GetCost( i );
				int adjacentSimpleIndex = simpleIndex + addFactor;
				int adjacentChunkIndex = ArrayAccessHelper::GetChunkIndex( adjacentSimpleIndex );

				if (adjacentSimpleIndex < 0 || adjacentSimpleIndex >= totalSize) continue;

				if (simpleIndex % m_MapBufferWidth == 0 && i % 3 == 0) continue;

				if (simpleIndex % m_MapBufferWidth == lastColumnIndex && i % 3 == 2) continue;

				if (!m_NavBuffer[adjacentChunkIndex]) continue;

				auto&& adjacentSegment = m_MapGraph->GetSegment( adjacentChunkIndex );
				segment->AddAdjacentSegment( adjacentSegment, cost );
				segment->SetPosition( Vector2D( simpleIndex % m_MapBufferWidth, simpleIndex / m_MapBufferWidth ) );
			}
		}
	}
}

int AIManager::Heuristic( Segment* segment1, Segment* segment2 )
{
	int x1 = ArrayAccessHelper::GetSimpleColumnIndex( segment1->GetIndex() );
	int y1 = ArrayAccessHelper::GetSimpleRowIndex( segment1->GetIndex() );
	int x2 = ArrayAccessHelper::GetSimpleColumnIndex( segment2->GetIndex() );
	int y2 = ArrayAccessHelper::GetSimpleRowIndex( segment2->GetIndex() );
	return abs( x1 - x2 ) + abs( y1 - y2 );
}

void AIManager::FindPath( Segment* start, Segment* target, CheckedSegments& cameFrom, Cost& cost )
{
	PriorityQueue<Segment*> frontier;
	frontier.Put( start, 0 );

	cameFrom[start] = start;
	cost[start] = 0;

	while (!frontier.Empty())
	{
		auto current = frontier.Get();

		if (current == target)
		{
			break;
		}

		for (auto&& next : current->Neighbors())
		{
			int newCost = cost[current] + next.second;

			Segment* nextSegment = next.first;
			if (!cost.count( nextSegment ) || newCost < cost[nextSegment])
			{
				cost[nextSegment] = newCost;
				int priority = newCost + Heuristic( nextSegment, target );
				frontier.Put( nextSegment, priority );
				cameFrom[nextSegment] = current;
			}
		}
	}
}

std::vector<Segment*> AIManager::ReconstructPath( Segment* start, Segment* target, CheckedSegments& cameFrom )
{
	std::vector<Segment*> path;

	Segment* current = target;
	path.push_back( current );

	while (current != start)
	{
		current = cameFrom[current];
		path.push_back( current );
	}

	std::reverse( path.begin(), path.end() );

	return path;
}