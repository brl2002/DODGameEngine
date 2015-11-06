#pragma once

enum class KeyCode
{
	KEY_W = 0x57,
	KEY_S = 0x53,
	KEY_A = 0x41,
	KEY_D = 0x44
};

class Input
{
	static bool m_InputsDown[];

public:
	static void Reset();

	static void Update( int keyCode );

	static bool InputDown( KeyCode keyCode );

	static bool InputUp( KeyCode keyCode );
};