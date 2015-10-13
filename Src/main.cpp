#include "Engine.h"

#include <Windows.h>

void main()
{
	HWND hwnd = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(hwnd, FALSE);
	EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);

	Engine engine = Engine();
	engine.Run();
}