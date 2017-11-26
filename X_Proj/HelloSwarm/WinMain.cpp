#include "Interceptor.h"

Ai::World aiWorld;
std::vector<Interceptor*> interceptors;

bool GameLoop(float deltaTime)
{
	if (X::IsKeyPressed(X::Keys::NUM_ADD))
	{
		Interceptor* interceptor = new Interceptor(aiWorld);
		interceptor->Load();
		interceptor->Position() = X::RandomVector2({ 0.0f,0.0f }, { static_cast<float>(X::GetScreenWidth()), static_cast<float>(X::GetScreenHeight()) });
		interceptors.push_back(interceptor);
	}

	for (auto interceptor : interceptors)
	{
		interceptor->Update(deltaTime);
	}
	for (auto interceptor : interceptors)
	{
		interceptor->Draw();
	}

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("xconfig.json");

	X::Run(GameLoop);

	X::Stop();
	return 0;
}