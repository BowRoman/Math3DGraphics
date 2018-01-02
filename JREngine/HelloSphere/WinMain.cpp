#include <Core\Inc\Core.h>
#include <Input\Inc\Input.h>
#include "GameApp.h"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	GameApp myApp;
	myApp.Initialize(hInstance, "Hello Sphere", 1024, 768);
	while (myApp.IsRunning())
	{
		myApp.Update();
	}
	myApp.Terminate();
	return 0;
}