#include "GameApp.h"
#include <assert.h>

namespace
{
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
	Graphics::TextureManager::StaticInitialize("../Assets/Images/");

	mTimer.Initialize();

	// camera
	mCamera.mTransform.SetPosition(Math::Vector3(0.0f, 6.0f, -10.0f));
	mCamera.mTransform.SetDirection(Math::Vector3(0.0f, 0.0f, 1.0f));

	// light
	mLight.direction = Math::Normalize(Math::Vector3(1.0f, -1.0f, 1.0f));
	mLight.ambient = Math::Vector4(0.4f, 0.4f, 0.4f, 1.0f);
	mLight.diffuse = Math::Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	mLight.specular = Math::Vector4(0.3f, 0.3f, 0.3f, 1.0f);

	mShadowConstantBuffer.Initialize();

	// Depth Map
	mDepthMapConstantBuffer.Initialize();
	mLinearSampler.Initialize(Graphics::Sampler::Filter::Linear, Graphics::Sampler::AddressMode::Clamp);
	mDepthMapVertexShader.Initialize(L"../Assets/Shaders/DepthMap.fx", Graphics::VertexPT::Format);
	mDepthMap.Initialize(1024, 1024);
	mLightCamera.mTransform.SetDirection(Math::Vector3(1.0f, -1.0f, 1.0f));
	mLightCamera.mTransform.SetPosition(Math::Vector3(-7.0f, 7.0f, -7.0f));

	// Plane Mesh
	mPlaneVertexShader.Initialize(L"../Assets/Shaders/ShadowMapping.fx", Graphics::Vertex::Format);
	mPlanePixelShader.Initialize(L"../Assets/Shaders/ShadowMapping.fx");
	mPlaneSpecTexture.Initialize(L"../Assets/Images/brickwork_spec.jpg");
	mPlaneNormalTexture.Initialize(L"../Assets/Images/brickwork_normal.jpg");
	mPlaneDisplacementTexture.Initialize(L"../Assets/Images/brickwork_bump.jpg");
	mPlaneTexture.Initialize(L"../Assets/Images/brickwork.jpg");
	Graphics::MeshBuilder::GeneratePlane(mPlane, 50.0f, 50.0f, -4.0f);
	mPlaneMeshBuffer.Initialize(mPlane.mVertices, sizeof(Graphics::Vertex), mPlane.mVertexCount, mPlane.mIndices, mPlane.mIndexCount);

	mVertexShader.Initialize(L"../Assets/Shaders/Texturing.fx", Graphics::Vertex::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/Texturing.fx");

	mSampler.Initialize(Graphics::Sampler::Filter::Point, Graphics::Sampler::AddressMode::Clamp);

	// model
	mModel.Load("../Assets/Models/Drone_red.txt");
	mConstantBuffer.Initialize();

	// Camera
	mCurrentCamera = &mCamera;
	mCurrentCamera->mTransform = mCamera.mTransform;

	mRotation = 0.0f;
}


void GameApp::OnTerminate()
{
	// Terminate
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mSampler.Terminate();

	mModel.Unload();
	mConstantBuffer.Terminate();

	// plane
	mPlanePixelShader.Terminate();
	mPlaneVertexShader.Terminate();
	mPlaneTexture.Terminate();
	mPlaneSpecTexture.Terminate();
	mPlaneNormalTexture.Terminate();
	mPlaneDisplacementTexture.Terminate();

	// depth
	mDepthMap.Terminate();
	mDepthMapConstantBuffer.Terminate();
	mDepthMapPixelShader.Terminate();
	mDepthMapVertexShader.Terminate();

	mShadowConstantBuffer.Terminate();

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
	const float cameraMoveSpeed = 10.0f;
	const float cameraTurnSpeed = 1.0f;
	if (is->IsKeyDown(Keys::W))
	{
		mCamera.mTransform.Walk(cameraMoveSpeed * mTimer.GetElapsedTime());
	}
	if (is->IsKeyDown(Keys::S))
	{
		mCamera.mTransform.Walk(-cameraMoveSpeed * mTimer.GetElapsedTime());
	}
	if (is->IsKeyDown(Keys::D))
	{
		mCamera.mTransform.Strafe(cameraMoveSpeed * mTimer.GetElapsedTime());
	}
	if (is->IsKeyDown(Keys::A))
	{
		mCamera.mTransform.Strafe(-cameraMoveSpeed * mTimer.GetElapsedTime());
	}
	if (is->IsKeyDown(Keys::E))
	{
		mCamera.mTransform.Rise(cameraMoveSpeed * mTimer.GetElapsedTime());
	}
	if (is->IsKeyDown(Keys::Q))
	{
		mCamera.mTransform.Rise(-cameraMoveSpeed * mTimer.GetElapsedTime());
	}

	if (is->IsMouseDown(Mouse::RBUTTON))
	{
		mCamera.mTransform.Yaw(is->GetMouseMoveX() * cameraTurnSpeed * mTimer.GetElapsedTime());
		mCamera.mTransform.Pitch(is->GetMouseMoveY() * cameraTurnSpeed * mTimer.GetElapsedTime());
	}
	if (is->IsKeyPressed(Keys::TAB))
	{
		if (mCurrentCamera == &mCamera)
		{
			mCurrentCamera = &mLightCamera;
			mCurrentCamera->mTransform = mLightCamera.mTransform;
		}
		else
		{
			mCurrentCamera = &mCamera;
			mCurrentCamera->mTransform = mCamera.mTransform;
		}
	}

	mRotation += mTimer.GetElapsedTime();

	GenerateDepthMap();

	DrawScene();
}

void GameApp::GenerateDepthMap()
{
	mDepthMap.BeginRender();

	Math::Matrix4 matView = mLightCamera.GetViewMatrix();
	Math::Matrix4 matProj = mLightCamera.GetProjectionMatrix(1);

	mDepthMapVertexShader.Bind();
	mDepthMapPixelShader.Bind();

	Math::Matrix4 matTrans = Math::Matrix4::Translation(0.0f, 0.0f, 0.0f);
	Math::Matrix4 matScale = Math::Matrix4::Scaling(0.05f);
	Math::Matrix4 matRot   = Math::Matrix4::RotationY(mRotation);
	Math::Matrix4 matWorld = matScale * matTrans * matRot;

	ConstantDepthData data;
	data.wvp = Math::Transpose(matWorld * matView * matProj);
	data.displacmentScale = 0.15f;
	mDepthMapConstantBuffer.Set(data);
	mDepthMapConstantBuffer.BindVS();
	mLinearSampler.BindVS(0);
	//mDepthMapConstantBuffer.BindPS();
	mDepthMapPixelShader.Bind();
	mDepthMapVertexShader.Bind();

	mModel.Render();

	matTrans = Math::Matrix4::Translation(120.0f, -2.0f, 10.0f);
	matScale = Math::Matrix4::Scaling(0.03f);
	matWorld = matRot * matScale * matTrans;

	data.wvp = Math::Transpose(matWorld * matView * matProj);
	data.displacmentScale = 0.15f;
	mDepthMapConstantBuffer.Set(data);
	mDepthMapConstantBuffer.BindVS();
	mLinearSampler.BindVS(0);
	//mDepthMapConstantBuffer.BindPS();
	mDepthMapPixelShader.Bind();
	mDepthMapVertexShader.Bind();

	mModel.Render();

	mDepthMap.EndRender();

}

void GameApp::DrawScene()
{
	Graphics::GraphicsSystem::Get()->BeginRender();

	Math::Matrix4 worldMatrix = Math::Matrix4::Scaling(1.0f);
	Math::Matrix4 viewMatrix = mCamera.GetViewMatrix(mCurrentCamera->mTransform);
	Math::Matrix4 CameraProjectionMatrix = mCamera.GetProjectionMatrix(Graphics::GraphicsSystem::Get()->GetAspectRatio());

	Math::Matrix4 LightViewMatrix = mLightCamera.GetViewMatrix();
	Math::Matrix4 LightProjectionMatrix = mLightCamera.GetProjectionMatrix(1);

	mSampler.BindVS(0);
	mSampler.BindPS(0);
	mSampler.BindVS(1);
	mSampler.BindPS(1);
	mVertexShader.Bind();
	mPixelShader.Bind();


	worldMatrix = Math::Matrix4::Scaling(0.05f) * Math::Matrix4::RotationY(mRotation);
	ConstantData data;
	data.wvp = Math::Transpose(worldMatrix * viewMatrix * CameraProjectionMatrix);
	mConstantBuffer.Set(data);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindPS();

	mModel.Render();

	worldMatrix = Math::Matrix4::RotationY(mRotation) * Math::Matrix4::Translation(120.0f, -2.0f, 10.0f) * Math::Matrix4::Scaling(0.03f);
	data.wvp = Math::Transpose(worldMatrix * viewMatrix * CameraProjectionMatrix);
	mConstantBuffer.Set(data);
	mConstantBuffer.BindVS();
	mConstantBuffer.BindPS();

	mModel.Render();

	//shadow map
	worldMatrix = Math::Matrix4::Scaling(1.0f);
	ConstantShadowMapData shadowData;
	shadowData.World = worldMatrix;
	shadowData.WVP = Math::Transpose(worldMatrix * viewMatrix * CameraProjectionMatrix);
	shadowData.WVPLight = Math::Transpose(worldMatrix * LightViewMatrix * LightProjectionMatrix);
	shadowData.ViewPosition = Math::Vector4(mCamera.mTransform.GetPosition(), 1.0f);
	shadowData.LightDirection = Math::Vector4(mLight.direction, 1.0f);
	shadowData.LightAmbient = mLight.ambient;
	shadowData.LightDiffuse = mLight.diffuse;
	shadowData.LightSpecular = mLight.specular;
	shadowData.MaterialAmbient = mMaterial.ambient;
	shadowData.MaterialDiffuse = mMaterial.diffuse;
	shadowData.MaterialSpecular = mMaterial.specular;
	shadowData.MaterialPower = mMaterial.specularPower;
	shadowData.DisplacementScale = 0.15f;
	mShadowConstantBuffer.Set(shadowData);
	mShadowConstantBuffer.BindVS();
	mShadowConstantBuffer.BindPS();

	mDepthMap.BindPS(4);


	mPlanePixelShader.Bind();
	mPlaneVertexShader.Bind();
	mPlaneTexture.BindPS(0);
	mPlaneTexture.BindVS(0);
	mPlaneSpecTexture.BindVS(1);
	mPlaneSpecTexture.BindPS(1);
	mPlaneNormalTexture.BindPS(2);
	mPlaneNormalTexture.BindVS(2);
	mPlaneDisplacementTexture.BindVS(3);

	mPlaneMeshBuffer.Render();

	for (int i = 0; i < 100; ++i)
	{
		Math::Vector3 p0(-50.0f, 0.0f, -50.0f + i);
		Math::Vector3 p1(+50.0f, 0.0f, -50.0f + i);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::Cyan());
	}
	for (int i = 0; i < 100; ++i)
	{
		Math::Vector3 p0(-50.0f + i, 0.0f, -50.0f);
		Math::Vector3 p1(-50.0f + i, 0.0f, +50.0f);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::Orange());
	}

	Graphics::SimpleDraw::Flush(viewMatrix * CameraProjectionMatrix);

	Graphics::GraphicsSystem::Get()->EndRender();
}