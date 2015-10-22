#include "AIManager.h"
#include "Common.h"
#include <vector>
#include <unordered_map>
#include <queue>
#include <tuple>

AIManager::AIManager( const bool* navBuffer, int mapBufferWidth, int mapBufferHeight )
	:	m_MapBufferWidth(mapBufferWidth),
		m_MapBufferHeight(mapBufferHeight),
		m_MapGraph(new Graph(m_MapBufferWidth * m_MapBufferHeight))
{
	// Use navBuffer to create navigation buffer array organized efficiently for AIManager
	m_NavBuffer = navBuffer;
}

AIManager::~AIManager()
{
	delete m_MapGraph;
}

void AIManager::Update( void* aiManagerInst, Entity* entities, int startIndex, int numEntities )
{
	AIManager* aiManager = (AIManager*)aiManagerInst;

	Graph* graph = aiManager->m_MapGraph;
	int width = aiManager->m_MapBufferWidth;

	for (int i = 1; i < numEntities; ++i)
	{
		int startIndex = entities[i].position.y * width + entities[i].position.x;
		Segment* startSegment = graph->GetSegment( ArrayAccessHelper::GetChunkIndex( startIndex ) );

		int endIndex = entities[0].position.y * width + entities[0].position.x;
		Segment* targetSegment = graph->GetSegment( ArrayAccessHelper::GetChunkIndex( endIndex ) );
	
		CheckedSegments checkedSegments;
		Cost cost;

		FindPath( startSegment, targetSegment, checkedSegments, cost);

		entities[i].path = ReconstructPath( startSegment, targetSegment, checkedSegments );
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
		auto&& segment = m_MapGraph->GetSegment(chunkIndex);
		segment->SetIndex(chunkIndex);

		int simpleIndex = ArrayAccessHelper::GetSimpleIndex(chunkIndex);

		for (int i = 0; i < 9; ++i)
		{
			if (i != 4)
			{
				int addFactor = ArrayAccessHelper::GetAddFactor(i);
				int cost = ArrayAccessHelper::GetCost(i);
				int adjacentSimpleIndex = simpleIndex + addFactor;
				int adjacentChunkIndex = ArrayAccessHelper::GetChunkIndex(adjacentSimpleIndex);

				if (adjacentSimpleIndex < 0 || adjacentSimpleIndex >= totalSize) continue;

				if (simpleIndex % m_MapBufferWidth == 0 && i % 3 == 0) continue;

				if (simpleIndex % m_MapBufferWidth == lastColumnIndex && i % 3 == 2) continue;

				auto&& adjacentSegment = m_MapGraph->GetSegment(adjacentChunkIndex);
				segment->AddAdjacentSegment(adjacentSegment, cost);
			}
		}
	}
}

int AIManager::Heuristic( Segment* segment1, Segment* segment2 )
{
	int x1 = ArrayAccessHelper::GetSimpleColumnIndex(segment1->GetIndex());
	int y1 = ArrayAccessHelper::GetSimpleRowIndex(segment1->GetIndex());
	int x2 = ArrayAccessHelper::GetSimpleColumnIndex(segment2->GetIndex());
	int y2 = ArrayAccessHelper::GetSimpleRowIndex(segment2->GetIndex());
	return abs(x1 - x2) + abs(y1 - y2);
}

void AIManager::FindPath( Segment* start, Segment* target, CheckedSegments& cameFrom, Cost& cost )
{
	PriorityQueue<Segment*> frontier;
	frontier.Put(start, 0);

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
			if (!cost.count(nextSegment) || newCost < cost[nextSegment])
			{
				cost[nextSegment] = newCost;
				int priority = newCost + Heuristic(nextSegment, target);
				frontier.Put(nextSegment, priority);
				cameFrom[nextSegment] = current;
			}
		}
	}
}

std::vector<Segment*> AIManager::ReconstructPath( Segment* start, Segment* target, CheckedSegments& cameFrom )
{
	std::vector<Segment*> path;
	Segment* current = target;
	path.push_back(current);
	while (current != start)
	{
		current = cameFrom[current];
		path.push_back(current);
	}
	std::reverse(path.begin(), path.end());
	return path;
}