#include "Engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Engine engine;

	bool result = engine.Initialize();
	if (result)
	{
		engine.Run();
	}

	engine.ShutdownEngine();

	return 0;
}