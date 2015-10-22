#pragma once

#include <vector>
#include "Graph.h"

class Vector2D
{
public:
	int x;
	int y;
};

class Entity
{
	char m_RenderCharacter;

public:
	Entity( char renderCharacter )
		: m_RenderCharacter(renderCharacter)
	{}

	inline char GetChar() { return m_RenderCharacter; }

	Vector2D position;
	Vector2D velocity;

	std::vector<Segment*> path;
};