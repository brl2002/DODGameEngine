#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	Player( char renderCharacter );

	void Update( float deltaTime );
};