#include "GameApp.h"
#include <assert.h>



namespace
{
	//
	// stack slice sphere
	uint32_t stacks = 512;
	uint32_t slices = 1024;
	//
	//

	const Graphics::VertexPT KVertices[] =
	{
		{ Math::Vector3(-1.0f,1.0f,0.0f),	Math::Vector2(0.0f, 0.0f) }, //0
		{ Math::Vector3(1.0f,1.0f,0.0f),	Math::Vector2(1.0f, 0.0f) }, //1
		{ Math::Vector3(-1.0f,-1.0f,0.0f),	Math::Vector2(0.0f, 1.0f) }, //2
		{ Math::Vector3(1.0f,-1.0f,0.0f),	Math::Vector2(1.0f, 1.0f) }, //3


	};
	const int kVertexCount = sizeof(KVertices) / sizeof(KVertices[0]);
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

	mCamera.mTransform.SetPosition(Math::Vector3(0.0f, 2.0f, -5.0f));
	mCamera.mTransform.SetDirection(Math::Vector3(0.0f, 0.0f, 1.0f));

	mLight.direction = Math::Normalize(Math::Vector3(1.0f, -1.0f, 1.0f));
	mLight.ambient = Math::Vector4(0.4f, 0.4f, 0.4f, 1.0f);
	mLight.diffuse = Math::Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	mLight.specular = Math::Vector4(0.3f, 0.3f, 0.3f, 1.0f);

	mMaterial.ambient = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.diffuse = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.specular = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.specularPower = 10.0f;


	//mConstantBuffer.Initialize();
	//mVertexShader.Initialize(L"../Assets/Shaders/Standard.fx", Graphics::Vertex::Format);
	//mPixelShader.Initialize(L"../Assets/Shaders/Standard.fx");


	mShadowConstantBuffer.Initialize();

	mVertexShader.Initialize(L"../Assets/Shaders/ShadowMapping.fx", Graphics::Vertex::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/ShadowMapping.fx");


	// Earth Mesh
	mTexture.Initialize(L"../Assets/Images/earth.jpg");
	mSpecMap.Initialize(L"../Assets/Images/earth_spec.jpg");
	mNormalMap.Initialize(L"../Assets/Images/earth_normal.jpg");
	mDisplacementMap.Initialize(L"../Assets/Images/earth_bump.jpg");
	mSampler.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Clamp);
	Graphics::MeshBuilder::GenerateStackSliceSphere(mMesh,stacks, slices, 1.0f);
	mMeshBuffer.Initialize(mMesh.mVertices, sizeof(Graphics::Vertex), mMesh.mNumVertices, mMesh.mIndices, mMesh.mNumIndices);
	
	// Plane Mesh
	mPlaneVertexShader.Initialize(L"../Assets/Shaders/ShadowMapping.fx", Graphics::Vertex::Format);
	mPlanePixelShader.Initialize(L"../Assets/Shaders/ShadowMapping.fx");
	mPlaneSpecTexture.Initialize(L"../Assets/Images/brickwork_spec.jpg");
	mPlaneNormalTexture.Initialize(L"../Assets/Images/brickwork_normal.jpg");
	mPlaneDisplacementTexture.Initialize(L"../Assets/Images/brickwork_bump.jpg");
	mPlaneTexture.Initialize(L"../Assets/Images/brickwork.jpg");
	Graphics::MeshBuilder::CreatePlane(mPlane, 10.0f, 10.0f, -2.0f);
	mPlaneMeshBuffer.Initialize(mPlane.mVertices, sizeof(Graphics::Vertex), mPlane.mNumVertices, mPlane.mIndices, mPlane.mNumIndices);

	// Depth Map
	mDepthMapConstantBuffer.Initialize();
	mLinearSampler.Initialize(Graphics::Sampler::Filter::Linear, Graphics::Sampler::AddressMode::Clamp);
	mDepthMapVertexShader.Initialize(L"../Assets/Shaders/DepthMap.fx", Graphics::VertexPT::Format);
	mDepthMap.Initialize(1024,1024); 
	mLightCamera.mTransform.SetDirection(Math::Vector3(1.0f, -1.0f, 1.0f));
	mLightCamera.mTransform.SetPosition(Math::Vector3(-5.0f, 5.0f, -5.0f));


	// Camera
	mCurrentCamera = &mCamera;
	mCurrentCamera->mTransform = mCamera.mTransform;

	// Post Process
	mVertexPostProcessShader.Initialize(L"../Assets/Shaders/PostProcess.fx", Graphics::VertexPT::Format);
	mPixelPostProcessShader.Initialize(L"../Assets/Shaders/PostProcess.fx");

	// Render Target
	mRenderTarget.Initialize(width, height, Graphics::RenderTarget::Format::RGBA_U8);
	mRenderTargetMeshBuffer.Initialize(KVertices, sizeof(Graphics::VertexPT), kVertexCount);
	mRotation = 0.0f;
}


void GameApp::OnTerminate()
{
	//Terminate
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mVertexPostProcessShader.Terminate();
	mPixelPostProcessShader.Terminate();

	mPlanePixelShader.Terminate();
	mPlaneVertexShader.Terminate();
	mPlaneTexture.Terminate();
	mPlaneSpecTexture.Terminate();
	mPlaneNormalTexture.Terminate();
	mPlaneDisplacementTexture.Terminate();

	mMeshBuffer.Terminate();
	mRenderTargetMeshBuffer.Terminate();
	//mConstantBuffer.Terminate();
	mTexture.Terminate();
	mSpecMap.Terminate();
	mNormalMap.Terminate();
	mDisplacementMap.Terminate();
	mSampler.Terminate();

	mRenderTarget.Terminate();
	mDepthMap.Terminate();
	mDepthMapConstantBuffer.Terminate();
	mDepthMapPixelShader.Terminate();
	mDepthMapVertexShader.Terminate();

	mShadowConstantBuffer.Terminate();

	Input::InputSystem::StaticTerminate();
	Graphics::SimpleDraw::StaticTerminate();
	Graphics::GraphicsSystem::StaticTerminate();

	UnhookWindow();
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
	const float cameraTurnSpeed = 0.3f;
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

	Math::Matrix4 matTrans = Math::Matrix4::Translation(0.0f,0.0f,0.0f);
	Math::Matrix4 matScale = Math::Matrix4::Scaling(1.0f);
	Math::Matrix4 matWorld = matScale * matTrans;

	ConstantDataDepth data;
	data.wvp = Math::Transpose(matWorld * matView * matProj);
	data.displacmentScale = 0.15f;
	mDepthMapConstantBuffer.Set(data);
	mDepthMapConstantBuffer.BindVS();
	mLinearSampler.BindVS(0);
	//mDepthMapConstantBuffer.BindPS();
	mDepthMapPixelShader.Bind();
	mDepthMapVertexShader.Bind();

	mMeshBuffer.Render();

	mDepthMap.EndRender();

}

void GameApp::DrawScene()
{
	mRenderTarget.BeginRender();

	//Math::Matrix4 rotationMatrix = Math::Matrix4::RotationY(mRotation);
	//Math::Matrix4 worldMatrix = rotationMatrix;
	Math::Matrix4 worldMatrix = Math::Matrix4();
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


	//shadow map
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

	mTexture.BindVS(0);
	mTexture.BindPS(0);
	mSpecMap.BindVS(1);
	mSpecMap.BindPS(1);
	mNormalMap.BindPS(2);
	mNormalMap.BindVS(2);
	mDisplacementMap.BindVS(3);
	mDepthMap.BindPS(4);


	mMeshBuffer.Render();


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






	for (int i = 0; i <= 100; ++i)
	{
		Math::Vector3 p0(-50.0f, 0.0f, -50.0f + i);
		Math::Vector3 p1(+50.0f, 0.0f, -50.0f + i);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::Orange());
	}
	for (int i = 0; i <= 100; ++i)
	{
		Math::Vector3 p0(-50.0f + i, 0.0f, -50.0f);
		Math::Vector3 p1(-50.0f + i, 0.0f, +50.0f);
		Graphics::SimpleDraw::DrawLine(p0, p1, Math::Vector4::Orange());
	}

	Graphics::SimpleDraw::Flush(viewMatrix * CameraProjectionMatrix);




	mRenderTarget.EndRender();

	Graphics::GraphicsSystem::Get()->BeginRender();
	mPixelPostProcessShader.Bind();
	mVertexPostProcessShader.Bind();
	mRenderTarget.BindPS(0);
	mRenderTargetMeshBuffer.Render();
	mRenderTarget.UnbindPS(0);
	Graphics::GraphicsSystem::Get()->EndRender();
}
