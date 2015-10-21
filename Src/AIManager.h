#pragma once

#include "Entity.h"

// AIManager responsible for managing and computing paths that entities are going to take.
// The manager uses A* pathfinding algorithm to find paths.
class AIManager
{
	const bool* m_NavBuffer;

	int m_MapBufferWidth;

	int m_MapBufferHeight;

public:
	AIManager(const bool* navBuffer, int mapBufferWidth, int mapBufferHeight);
	virtual ~AIManager();

	static void Update(void* aiManagerInst, Entity* entities,
		int startIndex, int numEntities);

	void SetupNavigationGraph(Graph* graph, int width,
		int height, int segmentWidth, int segmentHeight);

private:
	// Function to compute for cost of travelling between two given segments.
	inline int Heuristic(Segment* segment1, Segment* segment2);

	// Function to find path from start segment to target segment
	// using given graph structure.
	void FindPath(Graph* graph, Segment* start, Segment* target,
		std::unordered_map<Segment*, Segment*>& cameFrom,
		std::unordered_map<Segment*, int>& cost);

	std::vector<Segment*> ReconstructPath(Segment* start, Segment* target,
		std::unordered_map<Segment*, Segment*> cameFrom);
};