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


	const Graphics::VertexPT SkyBoxVertices[] =
	{
		{ Math::Vector3(-0.5f,-0.5f,0.0f),	Math::Vector2(1.0f, 0.0f) }, //0
		{ Math::Vector3(-0.5f,0.5f,0.0f),	Math::Vector2(0.0f, 0.0f) }, //1
		{ Math::Vector3(0.5f,0.5f,0.0f),	Math::Vector2(0.0f, 1.0f) }, //2
		{ Math::Vector3(0.5f,-0.5f,0.0f),	Math::Vector2(1.0f, 1.0f) }, //3

	};
	const int SkyBoxVertexCount = sizeof(SkyBoxVertices) / sizeof(SkyBoxVertices[0]);

	const uint32_t SkyBoxIndices[] =
	{
		2,1,0,
		0,2,3
	};
	const int SkyBoxIndexCount = sizeof(SkyBoxIndices) / sizeof(SkyBoxIndices[0]);
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

	mLight.direction = Math::Normalize(Math::Vector3(1.0f, -1.0f, 1.0f));
	mLight.ambient = Math::Vector4(0.1f, 0.1f, 0.1f, 1.0f);
	mLight.diffuse = Math::Vector4(0.6f, 0.6f, 0.6f, 1.0f);
	mLight.specular = Math::Vector4(0.3f, 0.3f, 0.3f, 1.0f);

	mMaterial.ambient = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.diffuse = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.specular = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.specularPower = 10.0f;


	mConstantBuffer.Initialize();
	mVertexShader.Initialize(L"../Assets/Shaders/Standard.fx", Graphics::Vertex::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/Standard.fx");

	/*mSkyBoxTexture.Initialize(L"../Assets/Images/SkyBoxes/SkyBox.dds");
	mSkyBoxVertexShader.Initialize(L"../Assets/Shaders/SkyBox.fx", Graphics::VertexPT::Format);
	mSkyBoxPixelShader.Initialize(L"../Assets/Shaders/SkyBox.fx");
	mSkyBoxMeshBuffer.Initialize(SkyBoxVertices, sizeof(Graphics::VertexPT), SkyBoxVertexCount, SkyBoxIndices, SkyBoxIndexCount);*/

	//mVertexPostProcessShader.Initialize(L"../Assets/Shaders/RadialDistortion.fx", Graphics::VertexPT::Format);
	//mPixelPostProcessShader.Initialize(L"../Assets/Shaders/RadialDistortion.fx");
	
	mVertexPostProcessShader.Initialize(L"../Assets/Shaders/PostProcess.fx", Graphics::VertexPT::Format);
	mPixelPostProcessShader.Initialize(L"../Assets/Shaders/PostProcess.fx");

	mTexture.Initialize(L"../Assets/Images/earth.jpg");
	mSpecMap.Initialize(L"../Assets/Images/earth_spec.jpg");
	mNormalMap.Initialize(L"../Assets/Images/earth_normal.jpg");
	mDisplacementMap.Initialize(L"../Assets/Images/earth_bump.jpg");


	mSampler.Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Clamp);

	mMeshBuilder.GenerateStackSliceSphere(mMesh,stacks, slices, 1.0f);

	mMeshBuffer.Initialize(mMesh.mVertices, sizeof(Graphics::Vertex), mMesh.mNumVertices, mMesh.mIndices, mMesh.mNumIndices);
		
	mRenderTarget.Initialize(width, height, Graphics::RenderTarget::Format::RGBA_U8);

	mRenderTargetMeshBuffer.Initialize(KVertices, sizeof(Graphics::VertexPT), kVertexCount);
}


void GameApp::OnTerminate()
{
	//Terminate
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mVertexPostProcessShader.Terminate();
	mPixelPostProcessShader.Terminate();

	//mSkyBoxTexture.Terminate();
	//mSkyBoxVertexShader.Terminate();
	//mSkyBoxPixelShader.Terminate();

	mMeshBuffer.Terminate();
	mRenderTargetMeshBuffer.Terminate();
	mConstantBuffer.Terminate();
	mTexture.Terminate();
	mSpecMap.Terminate();
	mNormalMap.Terminate();
	mDisplacementMap.Terminate();
	mSampler.Terminate();

	mRenderTarget.Terminate();


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



	mRenderTarget.BeginRender();



	Math::Matrix4 worldMatrix = Math::Matrix4::RotationY(mTimer.GetTotalTime() * 0.4f);
	Math::Matrix4 viewMatrix = mCamera.GetViewMatrix(mCameraTransform);
	Math::Matrix4 projectionMatrix = mCamera.GetProjectionMatrix(Graphics::GraphicsSystem::Get()->GetAspectRatio());

	mVertexShader.Bind();
	mPixelShader.Bind();
	mTexture.BindVS(0);
	mTexture.BindPS(0);
	mSpecMap.BindVS(1);
	mSpecMap.BindPS(1);
	mNormalMap.BindPS(2);
	mNormalMap.BindVS(2);
	mDisplacementMap.BindVS(3);
	


	ConstantData data;
	data.wvp = Math::Transpose(worldMatrix * viewMatrix * projectionMatrix);
	data.world = Math::Transpose(worldMatrix);
	data.cameraPosition = mCameraTransform.GetPosition();
	data.light = mLight;
	data.material = mMaterial;
	data.displacmentScale = 0.15f;
	data.view = Math::Transpose(viewMatrix);
	data.projection = Math::Transpose(projectionMatrix);

	mConstantBuffer.Set(data);
	mConstantBuffer.BindVS(); //matrix to vertex shader
	mConstantBuffer.BindPS();

	mMeshBuffer.Render();

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
	

	mRenderTarget.EndRender();

	Graphics::GraphicsSystem::Get()->BeginRender();
	mPixelPostProcessShader.Bind();
	mVertexPostProcessShader.Bind();

	mRenderTarget.BindPS(0);

	mRenderTargetMeshBuffer.Render();

	mRenderTarget.UnbindPS(0);


	Graphics::GraphicsSystem::Get()->EndRender();

}