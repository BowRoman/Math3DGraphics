#ifndef INCLUDED_CORE_WINDOW_H
#define INCLUDED_CORE_WINDOW_H

namespace Core
{

	class Window
	{
	public:
		Window();
		~Window();

		void Initialize(HINSTANCE instance, LPCSTR appName, int width, int height);
		void Terminate();

		bool ProcessMessage();

		HWND GetWindowHandle() const { return mWindow; }

	private:
		HINSTANCE mInstance;
		HWND mWindow;
		std::string mAppName;
	};

} // namespace Core

#endif // #ifndef INCLUDED_CORE_WINDOW_H