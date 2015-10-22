#pragma once

#include "Entity.h"
#include "Graph.h"
#include <unordered_map>

// AIManager responsible for managing and computing paths that entities are going to take.
// The manager uses A* pathfinding algorithm to find paths.
class AIManager
{
	typedef std::unordered_map<Segment*, Segment*> CheckedSegments;

	typedef std::unordered_map<Segment*, int> Cost;

	const bool* m_NavBuffer;

	int m_MapBufferWidth;

	int m_MapBufferHeight;

	Graph* m_MapGraph;

public:
	AIManager( const bool* navBuffer, int mapBufferWidth, int mapBufferHeight );
	
	virtual ~AIManager();

	void SetupNavigationGraph( int segmentWidth, int segmentHeight );

	static void Update( void* aiManagerInst, Entity* entities, int startIndex, int numEntities );

private:
	// Function to compute for cost of travelling between two given segments.
	static inline int Heuristic( Segment* segment1, Segment* segment2 );

	// Function to find path from start segment to target segment
	// using given graph structure.
	static void FindPath( Segment* start, Segment* target, CheckedSegments& cameFrom, Cost& cost );

	static std::vector<Segment*> ReconstructPath( Segment* start, Segment* target, CheckedSegments& cameFrom );
};