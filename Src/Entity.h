#pragma once

#include <vector>
#include "Common.h"
#include "Graph.h"

class Entity
{
	char m_RenderCharacter;

public:
	Entity(char renderCharacter)
		:	m_RenderCharacter(renderCharacter),
			position(Vector2D()),
			velocity(Vector2D())
	{}

	inline char GetChar() { return m_RenderCharacter; }

	virtual void Update( float deltaTime ) = 0;

	Vector2D position;
	Vector2D velocity;
};