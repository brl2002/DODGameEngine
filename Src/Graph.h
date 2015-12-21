#pragma once

#include <utility>
#include <vector>
#include <list>
#include <queue>
#include <functional>
#include "Common.h"

template<typename T, typename Number = int>
class PriorityQueue
{
	typedef std::pair<Number, T> PQElement;
	std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> elements;

public:
	inline bool Empty() { return elements.empty(); }

	inline void Put( T item, Number priority ) { elements.emplace(priority, item); }

	inline T Get()
	{
		T best_item = elements.top().second;
		elements.pop();
		return best_item;
	}
};

class Segment
{
public:
	typedef std::pair<Segment*, int> Neighbor;

private:
	std::vector<Neighbor> m_SegmentNeighbors;

	int m_Index;

	Vector2D m_Position;

public:
	inline void AddAdjacentSegment( Segment* segment, int cost ) { m_SegmentNeighbors.push_back( Neighbor( segment, cost ) ); }

	inline void SetIndex( int index ) { m_Index = index; }

	inline int GetIndex() { return m_Index; }

	inline void SetPosition( Vector2D& pos ) { m_Position = pos; }

	inline Vector2D GetPosition() { return m_Position; }

	inline const std::vector<Neighbor>& Neighbors() { return m_SegmentNeighbors; }
};

class Graph
{
	int m_Size;

	std::vector<Segment> m_Segments;

public:
	Graph::Graph( int size )
		: m_Size(size),
		m_Segments(std::vector<Segment>(m_Size))
	{
		for (int i = 0; i < m_Size; ++i)
		{
			m_Segments[i].SetIndex(i);
		}
	}

	Segment* GetSegment( int index ) { return &m_Segments[index]; }

	std::vector<Segment>& GetContainer() { return m_Segments; }
};