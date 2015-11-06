#include "Input.h"

bool Input::m_InputsDown[4] = { false, false, false, false };

void Input::Reset()
{
	// Resetting all input buffers.
	for (int i = 0; i < 4; ++i)
	{
		m_InputsDown[i] = false;
	}
}

void Input::Update( int keyCode )
{
	switch (keyCode)
	{
	case (int)KeyCode::KEY_W:
	{
		m_InputsDown[0] = true;
	}
	break;

	case (int)KeyCode::KEY_S:
	{
		m_InputsDown[1] = true;
	}
	break;

	case (int)KeyCode::KEY_A:
	{
		m_InputsDown[2] = true;
	}
	break;

	case (int)KeyCode::KEY_D:
	{
		m_InputsDown[3] = true;
	}
	break;
	}
}

bool Input::InputDown( KeyCode keyCode )
{
	switch (keyCode)
	{
	case KeyCode::KEY_W:
	{
		return m_InputsDown[0];
	}
	break;

	case KeyCode::KEY_S:
	{
		return m_InputsDown[1];
	}
	break;

	case KeyCode::KEY_A:
	{
		return m_InputsDown[2];
	}
	break;

	case KeyCode::KEY_D:
	{
		return m_InputsDown[3];
	}
	break;
	}

	return false;
}

bool Input::InputUp( KeyCode keyCode )
{
	switch (keyCode)
	{
	case KeyCode::KEY_W:
	{
		return !m_InputsDown[0];
	}
	break;

	case KeyCode::KEY_S:
	{
		return !m_InputsDown[1];
	}
	break;

	case KeyCode::KEY_A:
	{
		return !m_InputsDown[2];
	}
	break;

	case KeyCode::KEY_D:
	{
		return !m_InputsDown[3];
	}
	break;
	}

	return false;
}