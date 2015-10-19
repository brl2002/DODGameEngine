#include "AIManager.h"

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

void AIManager::Update(void* aiManagerInst, Entity* entities, int startIndex, int numEntities)
{

}