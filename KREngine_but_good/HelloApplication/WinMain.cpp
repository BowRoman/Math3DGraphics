#include <Core\Inc\Core.h>
#include <Input\Inc\Input.h>


class GameApp : public Core::Application
{
private:

	void OnInitialize(uint32_t width, uint32_t height) override
	{
		mWindow.Initialize(GetInstance(), GetAppName(), width, height);
		HookWindow(mWindow.GetWindowHandle());
		Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());
	}
	void OnTerminate() override
	{
		Input::InputSystem::StaticTerminate();
		UnHookWindow();
		mWindow.Terminate();
	}
	void OnUpdate() override
	{
		if (mWindow.ProcessMessage())
		{
			Kill();
		}

		Input::InputSystem* is = Input::InputSystem::Get();
		is->Update();
		if (is->IsKeyPressed(Keys::ESCAPE))
		{
			PostQuitMessage(0);
		}
	}
	Core::Window mWindow;

};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
	GameApp myApp;
	myApp.Initialize(hInstance, "Hello Application", 1024, 768);
	while (myApp.IsRunning())
	{
		myApp.Update();
	}
	myApp.Terminate();
	return 0;
}