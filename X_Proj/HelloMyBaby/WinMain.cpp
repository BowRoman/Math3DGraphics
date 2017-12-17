#include "Constant.h"
#include "Interceptor.h"

Ai::World aiWorld;
std::vector<Interceptor*> interceptors;
FoodContainer* foodContainer;

bool GameLoop(float deltaTime)
{
	if (X::IsKeyPressed(X::Keys::NUM_ADD))
	{
		Interceptor* interceptor = new Interceptor(aiWorld, foodContainer);
		interceptor->Load();
		interceptor->Position() = X::RandomVector2({ 0.0f,0.0f }, { static_cast<float>(X::GetScreenWidth()), static_cast<float>(X::GetScreenHeight()) });
		interceptors.push_back(interceptor);
	}

	if (X::IsKeyPressed(X::Keys::ONE))
	{
		int foodIdx = foodContainer->GetFood();
		if (foodIdx != -1)
		{
			X::Math::Vector2 pos = X::RandomVector2();
			pos.y *= X::GetScreenHeight() - 1.0f;
			pos.x *= X::GetScreenWidth() - 1.0f;
			foodContainer->At(foodIdx)->mPosition = pos;
		}
	}
	foodContainer->Draw();

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
	foodContainer = new FoodContainer();
	X::Run(GameLoop);

	X::Stop();
	return 0;
}