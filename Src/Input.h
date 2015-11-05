#pragma once

class InputComponent
{
	bool* m_InputsDown;

public:
	InputComponent();

	virtual ~InputComponent();

	void Update();

	bool InputDown();

	bool InputUp();
};