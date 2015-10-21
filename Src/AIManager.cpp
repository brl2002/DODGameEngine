#include "AIManager.h"
#include "Graph.h"
#include "Common.h"
#include <vector>
#include <unordered_map>
#include <queue>
#include <tuple>

AIManager::AIManager(const bool* navBuffer, int mapBufferWidth, int mapBufferHeight)
	:	m_MapBufferWidth(mapBufferWidth),
		m_MapBufferHeight(mapBufferHeight)
{
	// Use navBuffer to create navigation buffer array organized efficiently for AIManager
	m_NavBuffer = navBuffer;


}

AIManager::~AIManager()
{

}

void AIManager::Update(
	void* aiManagerInst,
	Entity* entities,
	int startIndex,
	int numEntities)
{

}

void AIManager::SetupNavigationGraph(
	Graph* graph,
	int width, int height,
	int segmentWidth, int segmentHeight)
{
	// From chunk index to linear index and use look-up table to find addition value;
	// and once you add that value to the linear index, convert it back to chunk index

	int totalSize = width * height;
	int lastColumnIndex = width - 1;

	for (int chunkIndex = 0; chunkIndex < totalSize; ++chunkIndex)
	{
		auto&& segment = graph->GetSegment(chunkIndex);
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

				if (simpleIndex % width == 0 && i % 3 == 0) continue;

				if (simpleIndex % width == lastColumnIndex && i % 3 == 2) continue;

				auto&& adjacentSegment = graph->GetSegment(adjacentChunkIndex);
				segment->AddAdjacentSegment(adjacentSegment, cost);
			}
		}
	}
}

int AIManager::Heuristic(Segment* segment1, Segment* segment2)
{
	int x1 = ArrayAccessHelper::GetSimpleColumnIndex(segment1->GetIndex());
	int y1 = ArrayAccessHelper::GetSimpleColumnIndex(segment1->GetIndex());
	int x2 = ArrayAccessHelper::GetSimpleColumnIndex(segment2->GetIndex());
	int y2 = ArrayAccessHelper::GetSimpleColumnIndex(segment2->GetIndex());
	return abs(x1 - x2) + abs(y1 - y2);
}

void AIManager::FindPath(Graph* graph, Segment* start, Segment* target,
	std::unordered_map<Segment*, Segment*>& cameFrom,
	std::unordered_map<Segment*, int>& cost)
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

std::vector<Segment*> AIManager::ReconstructPath(Segment* start, Segment* target,
	std::unordered_map<Segment*, Segment*> cameFrom)
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