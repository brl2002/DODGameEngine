#include "Enemy.h"
#include "Common.h"

// Constructor
Enemy::Enemy( char renderCharacter, float speed )
	:	Entity(renderCharacter),
		currentPathIndex(0),
		currentTaskIndex(0),
		m_ShouldFindPath(true),
		m_Speed(speed),
		m_CheckPathRate(1.0f),
		m_CurrentCheckPathTime(0)
{}

// Desctructor
Enemy::~Enemy()
{}

void Enemy::Update( double deltaTime )
{
	m_CurrentCheckPathTime += deltaTime;

	if (m_CurrentCheckPathTime > m_CheckPathRate)
	{
		m_CurrentCheckPathTime = 0;

		//m_ShouldFindPath = true;
	}
}