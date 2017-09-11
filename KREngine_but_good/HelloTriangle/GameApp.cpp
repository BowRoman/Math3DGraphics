#include "GameApp.h"
#include <assert.h>
namespace
{
	
	const Graphics::VertexPC KVertices[] =
	{
		{ Math::Vector3(-0.9f,0.4f,0.0f), Math::Vector4::Cyan() }, //0
		{ Math::Vector3(-0.45f,0.9f,0.0f),Math::Vector4::Cyan() }, //1
		{ Math::Vector3(0.0f,0.4f,0.0f),Math::Vector4::Cyan() }, //2
		{ Math::Vector3(0.45f,0.9f,0.0f),Math::Vector4::Cyan() }, //3
		{ Math::Vector3(0.9f,0.4f,0.0f),Math::Vector4::Cyan() }, //4
		{ Math::Vector3(0.9f,-0.2f,0.0f) ,Math::Vector4::Cyan() }, //5
		{ Math::Vector3(-0.9f,-0.2f,0.0f),Math::Vector4::Blue() }, //6
		{ Math::Vector3(0.0f,-0.9f,0.0f),Math::Vector4::Blue() }, //7

	};
	const int kVertexCount = sizeof(KVertices) / sizeof(KVertices[0]);

	const uint32_t kIndices[] =
	{
		0,1,2,
		2,3,4,
		0,4,5,
		0,5,6,
		6,5,7
	};
	const int kIndexCount = sizeof(kIndices) / sizeof(kIndices[0]);
}

GameApp::GameApp()
{
}

GameApp::~GameApp()
{
}

void GameApp::OnInitialize(uint32_t width, uint32_t height)
{
	mWindow.Initialize(GetInstance(), GetAppName(), width, height);
	HookWindow(mWindow.GetWindowHandle());

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());
	mMeshBuffer.Initialize(KVertices, sizeof(Graphics::VertexPC), kVertexCount, kIndices, kIndexCount);
	mVertexShader.Initialize(L"../Assets/Shaders/ShaderVertexPC.fx", Graphics::VertexPC::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/ShaderVertexPC.fx");
}


void GameApp::OnTerminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mMeshBuffer.Terminate();
	//Terminate

	Input::InputSystem::StaticTerminate();
	Graphics::GraphicsSystem::StaticTerminate();

	UnHookWindow();
	mWindow.Terminate();
}


void GameApp::OnUpdate()
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

	Graphics::GraphicsSystem::Get()->BeginRender();

	mVertexShader.Bind();
	mPixelShader.Bind();
	mMeshBuffer.Render();

	Graphics::GraphicsSystem::Get()->EndRender();
}