#pragma once

#include <tuple>
#include <vector>
#include <list>
#include "Entity.h"

class Graph
{
	std::list<Segment> m_SegElements;

public:
	void Add(Segment&& segment)
	{
		m_SegElements.push_back(segment);
	}
};

//std::get<0>(*adjacentSegments.begin()->begin());
typedef std::tuple<Segment*, int, int> link;

class Segment
{
public:
	std::vector<std::vector<link>> adjacentSegments;
};

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

	static void Update(void* aiManagerInst, Entity* entities, int startIndex, int numEntities);
};