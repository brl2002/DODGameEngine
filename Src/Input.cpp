#include "Input.h"

InputComponent::InputComponent()
	:	m_InputsDown(new bool[4])
{}

InputComponent::~InputComponent()
{
	delete m_InputsDown;
}