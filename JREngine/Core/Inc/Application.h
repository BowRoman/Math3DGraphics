#pragma once


namespace Core
{

class Application
{
public:
	Application();
	virtual ~Application();
	void Initialize(HINSTANCE instance, LPCSTR appName, uint32_t width, uint32_t height);
	void Terminate();

	void HookWindow(HWND hWnd);
	void UnhookWindow();

	void Update();

	void Kill() { mRunning = false; }

	HINSTANCE GetInstance() const { return mInstance; }
	HWND GetWindow() const { return mWindow; }
	const char* GetAppName() const { return mAppName.c_str(); }
	bool IsRunning() { return mRunning; }
private:
	virtual void OnInitialize(uint32_t width, uint32_t height) = 0;
	virtual void OnTerminate() = 0;
	virtual void OnUpdate() = 0;
private:
	HINSTANCE mInstance;
	HWND mWindow;
	std::string mAppName;
	bool mRunning;
}; // class Application

} //namespace Core