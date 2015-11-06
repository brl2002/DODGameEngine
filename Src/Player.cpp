#include "Player.h"

Player::Player( char renderCharacter, float speed )
	:	Entity( renderCharacter ),
		m_Speed(speed)
{}

void Player::Update( double deltaTime )
{

}

void Player::MoveHorizontally( float value )
{
	velocity.x = value * m_Speed;
}

void Player::MoveVertically( float value )
{
	velocity.y = value * m_Speed;
}