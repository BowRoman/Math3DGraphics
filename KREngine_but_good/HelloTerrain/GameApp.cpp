#include "GameApp.h"
#include <assert.h>

// http://www.chadvernon.com/blog/resources/directx9/terrain-generation-with-a-heightmap/

namespace
{
	Graphics::Vertex KVertices[512 * 512];
	int kVertexCount = sizeof(KVertices) / sizeof(KVertices[0]);

	uint32_t* Indices = nullptr;
	int IndexCount;


	uint32_t stacks = 20;
	uint32_t slices = 20;
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
	Graphics::SimpleDraw::StaticInitialize(10000);
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	mTimer.Initialize();

	mCameraTransform.SetPosition(Math::Vector3(0.0f, 2.0f, -5.0f));
	mCameraTransform.SetDirection(Math::Vector3(0.0f, 0.0f, 1.0f));


	mConstantBuffer.Initialize();
	mTerrainVertexShader.Initialize(L"../Assets/Shaders/Texturing.fx", Graphics::Vertex::Format);
	mTerrainPixelShader.Initialize(L"../Assets/Shaders/Texturing.fx");

	mSphereVertexShader.Initialize(L"../Assets/Shaders/Texturing.fx", Graphics::Vertex::Format);
	mSpherePixelShader.Initialize(L"../Assets/Shaders/Texturing.fx");

	mHeightMap.Initialize("../Assets/HeightMaps/HeightMap.raw", 512, 512);

	unsigned int cols = mHeightMap.GetColumns();

	for (uint32_t i = 0; i < cols; ++i)
	{
		for (uint32_t j = 0; j < mHeightMap.GetRows(); ++j)
		{
			KVertices[(i * cols) + j].position.y = mHeightMap.GetHeight(i, j) * 10.0f;
			KVertices[(i * cols) + j].position.x = j * 0.2f;
			KVertices[(i * cols) + j].position.z = i * 0.2f;
			KVertices[(i * cols) + j].uv.x = (float)j / (float)cols;
			KVertices[(i * cols) + j].uv.y = (float)i / (float)cols;
		}
	}
	mHeightMap.GenerateIndices(&Indices, IndexCount);

	mMeshBuilder.GenerateStackSliceSphere(mMesh, stacks, slices, 1.0f);

	mTerrainTexture.Initialize(L"../Assets/Images/Ice.jpg");
	mSphereTexture.Initialize(L"../Assets/Images/earth.jpg");
	mTerrainSampler.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);
	mSphereSampler.Initialize(Graphics::Sampler::Filter::Point, Graphics::Sampler::AddressMode::Wrap);

	mSphereMeshBuffer.Initialize(mMesh.mVertices, sizeof(Graphics::Vertex), mMesh.mVertexCount, mMesh.mIndices, mMesh.mIndexCount);

	mTerrainMeshBuffer.Initialize(KVertices, sizeof(Graphics::Vertex), kVertexCount, Indices, IndexCount);
	mTerrainMeshBuffer.SetTopology(Graphics::MeshBuffer::Topology::TriangleStrip);

}


void GameApp::OnTerminate()
{
	mTerrainVertexShader.Terminate();
	mSphereVertexShader.Terminate();

	mTerrainPixelShader.Terminate();
	mSpherePixelShader.Terminate();


	mTerrainMeshBuffer.Terminate();
	mSphereMeshBuffer.Terminate();

	mConstantBuffer.Terminate();

	mTerrainTexture.Terminate();
	mTerrainSampler.Terminate();

	mSphereTexture.Terminate();
	mSphereSampler.Terminate();

	Input::InputSystem::StaticTerminate();
	Graphics::SimpleDraw::StaticTerminate();
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
	mTimer.Update();

	Input::InputSystem* is = Input::InputSystem::Get();
	is->Update();
	if (is->IsKeyPressed(Keys::ESCAPE))
	{
		PostQuitMessage(0);
	}
	const float fastCameraSpeed = 25.0f;
	float cameraMoveSpeed = 10.0f;
	const float cameraTurnSpeed = 1.0f;
	if (is->IsKeyDown(Keys::LSHIFT))
	{
		cameraMoveSpeed += fastCameraSpeed;
	}
	if (is->IsKeyDown(Keys::W))
	{
		mCameraTransform.Walk(cameraMoveSpeed * mTimer.GetElapsedTime());
	}
	if (is->IsKeyDown(Keys::S))
	{
		mCameraTransform.Walk(-cameraMoveSpeed * mTimer.GetElapsedTime());
	}
	if (is->IsKeyDown(Keys::D))
	{
		mCameraTransform.Strafe(cameraMoveSpeed * mTimer.GetElapsedTime());
	}
	if (is->IsKeyDown(Keys::A))
	{
		mCameraTransform.Strafe(-cameraMoveSpeed * mTimer.GetElapsedTime());
	}
	if (is->IsMouseDown(Mouse::RBUTTON))
	{
		mCameraTransform.Yaw(is->GetMouseMoveX() * cameraTurnSpeed * mTimer.GetElapsedTime());
		mCameraTransform.Pitch(is->GetMouseMoveY() * cameraTurnSpeed * mTimer.GetElapsedTime());
	}

	Graphics::GraphicsSystem::Get()->BeginRender();

	Math::Matrix4 worldMatrix = Math::Matrix4::RotationY(mTimer.GetTotalTime());
	Math::Matrix4 viewMatrix = mCamera.GetViewMatrix(mCameraTransform);
	Math::Matrix4 projectionMatrix = mCamera.GetProjectionMatrix(Graphics::GraphicsSystem::Get()->GetAspectRatio());

	ConstantData data;
	data.wvp = Math::Transpose(viewMatrix * projectionMatrix);
	mConstantBuffer.Set(data);
	mConstantBuffer.BindVS(); //matrix to vertex shader



	mTerrainVertexShader.Bind();
	mTerrainPixelShader.Bind();
	mTerrainSampler.BindPS(0);
	mTerrainTexture.BindPS(0);
	mTerrainMeshBuffer.Render();


	mSphereVertexShader.Bind();
	mSpherePixelShader.Bind();
	mSphereSampler.BindPS(0);
	mSphereTexture.BindPS(0);
	mSphereMeshBuffer.Render();



	for (int i = 0; i < 100; ++i)
	{
		Math::Vector3 p0(-50.0f, 0.0f, -50.0f + i);
		Math::Vector3 p1(+50.0f, 0.0f, -50.0f + i);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::White());
	}
	for (int i = 0; i < 100; ++i)
	{
		Math::Vector3 p0(-50.0f + i, 0.0f, -50.0f);
		Math::Vector3 p1(-50.0f + i, 0.0f, +50.0f);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::White());
	}

	Graphics::SimpleDraw::Flush(viewMatrix * projectionMatrix);

	Graphics::GraphicsSystem::Get()->EndRender();
}