#pragma once

#include "Entity.h"

class Player : public Entity
{
	float m_Speed;

public:
	Player( char renderCharacter, float speed );

	void Update( double deltaTime );

	void MoveHorizontally( float value );

	void MoveVertically( float value );
};