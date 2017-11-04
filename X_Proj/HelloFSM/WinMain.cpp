#include <Ai.h>
#include <XEngine.h>

bool GameLoop(float deltaTime)
{

}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("xconfig.json");
	X::Run(GameLoop);
	X::Stop();
	return 0;
}