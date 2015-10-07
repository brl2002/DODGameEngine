#pragma once

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
	Entity(char renderCharacter) : m_RenderCharacter(renderCharacter) {}

	inline char GetChar() { return m_RenderCharacter; }

	Vector2D position;
	Vector2D velocity;
};