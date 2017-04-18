#include <Core\Inc\Core.h>
#include <Input\Inc\Input.h>

class GameApp : public Core::Application
{
private:
	virtual void OnInitialize(uint32_t width, uint32_t height) override
	{
		mWindow.Initialize(GetInstance(), GetAppName(), width, height);
		HookWindow(mWindow.GetWindowHandle());

		Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());
	}

	virtual void OnTerminate() override
	{
		Input::InputSystem::StaticTerminate();

		UnhookWindow();
		mWindow.Terminate();
	}

	virtual void OnUpdate() override
	{
		if (mWindow.ProcessMessage())
		{
			Kill();
		}

		Input::InputSystem* iS = Input::InputSystem::Get();
		iS->Update();

		if (iS->IsKeyPressed(Keys::ESCAPE))
		{
			PostQuitMessage(0);
		}
	}

private:
	Core::Window mWindow;
	
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GameApp myApp;
	myApp.Initialize(hInstance, "Hello application.", 1024, 768);

	while (myApp.IsRunning())
	{
		myApp.Update();
	}

	myApp.Terminate();

	return 0;
}