#include <Ai.h>
#include <XEngine.h>

#include "SCV.h"
#include "Constant.h"

SCV scv;
SCV scv2;
SCV* selectedUnit = nullptr;
X::TextureId depositCenterId;
X::TextureId resourceId;


bool GameLoop(float deltaTime)
{
	X::DrawSprite(depositCenterId, kDepositLocation - X::Math::Vector2(64,64));
	X::DrawSprite(resourceId, kResourceLocation - X::Math::Vector2(16, 16));

	float destX = (float)X::GetMouseScreenX();
	float destY = (float)X::GetMouseScreenY();
	if (X::IsMousePressed(X::Mouse::LBUTTON))
	{
		if (X::Math::Distance(scv.GetPosition(), { destX,destY }) < 32.0f)
		{
			selectedUnit = &scv;
		}
		else if (X::Math::Distance(scv2.GetPosition(), { destX,destY }) < 32.0f)
		{
			selectedUnit = &scv2;
		}
	}
	if (X::IsMousePressed(X::Mouse::RBUTTON))
	{
		if (selectedUnit)
		{
			selectedUnit->SetDestination({ destX,destY });
		}
	}

	scv.Update(deltaTime);
	scv.Draw();

	scv2.Update(deltaTime);
	scv2.Draw();
	
	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("xconfig.json");
	scv.Load();
	scv2.Load();
	scv2.SetPosition(X::Math::Vector2(600.0f, 500.0f));
	scv2.SetDestination(X::Math::Vector2(600.0f, 500.0f));

	depositCenterId = X::LoadTexture("refinery_04.png");
	resourceId = X::LoadTexture("qmark.png");

	X::Run(GameLoop);
	X::Stop();
	return 0;
}