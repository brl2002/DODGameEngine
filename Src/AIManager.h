#pragma once

// AIManager responsible for managing and computing paths that entities are going to take.
// The manager uses A* pathfinding algorithm to find paths.
class AIManager
{
	bool* m_NavBuffer;

	int m_MapBufferWidth;

	int m_MapBufferHeight;

public:
	AIManager(const bool* navBuffer, int mapBufferWidth, int mapBufferHeight);
	virtual ~AIManager();
};