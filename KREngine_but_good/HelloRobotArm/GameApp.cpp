#include "GameApp.h"
#include <assert.h>



namespace
{
	// world matrix
	Math::Matrix4 worldMatrix;
	// bone data
	struct Bone
	{
		Math::Vector3 translation;
		Math::Quaternion rotation;
		Bone* parent;
		Math::Matrix4 transform;
		Bone() : parent(nullptr), transform(Math::Matrix4::Identity()) {}
		Bone(Math::Vector3 trans, Math::Quaternion rot, Bone* par = nullptr) : translation(trans), rotation(rot), parent(par), transform(Math::Matrix4::Identity()) {}
		Math::Matrix4 GenerateTransform()
		{
			if (parent) // grab the parent space
			{
				transform = parent->GenerateTransform();
			}
			else // reset the transform
			{
				transform = Math::Matrix4::Identity();
			}
			Math::Matrix4 newTransform = Math::Matrix4::Scaling(1.0f,1.0f,1.0f) * Math::Matrix4::RotationQuaternion(rotation) * Math::Matrix4::Translation(translation);
			transform = newTransform * transform;
			return transform;
		}
	};
	std::vector<Bone*> kBones;
	std::vector<Math::Matrix4> GetWorldTransforms(std::vector<Bone*> bones)
	{
		std::vector<Math::Matrix4> worldTransforms;
		worldTransforms.reserve(bones.size());
		for (size_t i =0;i<bones.size();++i)
		{
			bones[i]->transform = worldMatrix * bones[i]->transform;
			worldTransforms.push_back(bones[i]->transform);
		}
		return worldTransforms;
	}
	Bone hand;
	Bone forearm;
	Bone bicep;
	// vertex data
	const Graphics::VertexPC kVertices[] =
	{
		{ Math::Vector3(-0.5f, +3.0f, -0.5f), Math::Vector4::Red() },					// 0
		{ Math::Vector3(+0.5f, +3.0f, -0.5f), Math::Vector4::Orange() },				// 1
		{ Math::Vector3(-0.5f, +0.0f, -0.5f), Math::Vector4::Yellow() },				// 2
		{ Math::Vector3(+0.5f, +0.0f, -0.5f), Math::Vector4::Lime() },					// 3
																						
		{ Math::Vector3(-0.5f, +3.0f, +0.5f), Math::Vector4::Green() },					// 4
		{ Math::Vector3(+0.5f, +3.0f, +0.5f), Math::Vector4::Cyan() },					// 5
		{ Math::Vector3(-0.5f, +0.0f, +0.5f), Math::Vector4::Magenta() },				// 6
		{ Math::Vector3(+0.5f, +0.0f, +0.5f), Math::Vector4::Blue() },					// 7

		/*
			   4------5
			  /|     /|
			 / 6----/-7
			0------1 /
		    |/     |/
			2------3
		*/
	};
	const int kVertexCount = sizeof(kVertices) / sizeof(kVertices[0]);

	// index data
	const uint32_t kIndices[]
	{
		// front
		2, 0, 1,
		1, 3, 2,

		// back
		5, 4, 6,
		6, 7, 5,

		// top
		0, 4, 5,
		5, 1, 0,

		// bottom
		7, 6, 2,
		2, 3, 7,

		// left
		4, 0, 2,
		2, 6, 4,

		// right
		1, 5, 7,
		7, 3, 1

	};
	const int kIndexCount = sizeof(kIndices) / sizeof(kIndices[0]);

	// basic sphere variables
	uint32_t numRings = 6, numSlices = 6;
	float scale = 1.f;
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

	mCameraTransform.SetPosition(Math::Vector3(0.0f, 10.0f, -10.0f));
	mCameraTransform.SetDirection(Math::Vector3(0.0f, 0.0f, 1.0f));


	mConstantBuffer.Initialize();
	mVertexShader.Initialize(L"../Assets/Shaders/SimpleDraw.fx", Graphics::VertexPC::Format);
	mPixelShader.Initialize(L"../Assets/Shaders/SimpleDraw.fx");
	mMeshBuffer.Initialize(kVertices, sizeof(Graphics::VertexPC), kVertexCount, kIndices, kIndexCount);

}


void GameApp::OnTerminate()
{
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mMeshBuffer.Terminate();
	mConstantBuffer.Terminate();
	//Terminate

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
	const float baseCameraMoveSpeed = 10.0f;
	float cameraMoveSpeed = baseCameraMoveSpeed;
	const float cameraTurnSpeed = 1.0f;
	float dTime = mTimer.GetElapsedTime();
	if (is->IsKeyDown(Keys::LSHIFT))
	{
		cameraMoveSpeed = baseCameraMoveSpeed * 2.0;
	}
	if (is->IsKeyDown(Keys::W))
	{
		mCameraTransform.Walk(cameraMoveSpeed * dTime);
	}
	if (is->IsKeyDown(Keys::S))
	{
		mCameraTransform.Walk(-cameraMoveSpeed * dTime);
	}
	if (is->IsKeyDown(Keys::D))
	{
		mCameraTransform.Strafe(cameraMoveSpeed * dTime);
	}
	if (is->IsKeyDown(Keys::A))
	{
		mCameraTransform.Strafe(-cameraMoveSpeed * dTime);
	}
	if (is->IsKeyDown(Keys::E))
	{
		mCameraTransform.Rise(cameraMoveSpeed * dTime);
	}
	if (is->IsKeyDown(Keys::Q))
	{
		mCameraTransform.Rise(-cameraMoveSpeed * dTime);
	}

	if (is->IsMouseDown(Mouse::RBUTTON))
	{
		mCameraTransform.Yaw(is->GetMouseMoveX() * cameraTurnSpeed * dTime);
		mCameraTransform.Pitch(is->GetMouseMoveY() * cameraTurnSpeed * dTime);
	}

	// bone control
	if (is->IsKeyDown(Keys::O))
	{
		bendWrist += 1.0 * dTime;
	}
	if (is->IsKeyDown(Keys::I))
	{
		bendWrist -= 1.0 * dTime;
	}
	if (is->IsKeyDown(Keys::K))
	{
		bendElbow += 1.0 * dTime;
	}
	if (is->IsKeyDown(Keys::J))
	{
		bendElbow -= 1.0 * dTime;
	}
	if (is->IsKeyDown(Keys::M))
	{
		bendShoulder += 1.0 * dTime;
	}
	if (is->IsKeyDown(Keys::N))
	{

		bendShoulder -= 1.0 * dTime;
	}

	Graphics::GraphicsSystem::Get()->BeginRender();

	bicep.rotation = Math::Quaternion::RotationAxis(Math::Vector3::ZAxis(), bendShoulder);
	bicep.translation = Math::Vector3(0.0f, 0.0f, 0.0f);
	forearm.rotation = Math::Quaternion::RotationAxis(Math::Vector3::ZAxis(), bendElbow);
	forearm.translation = Math::Vector3(0.0f, 3.0f, 0.0f);
	forearm.parent = &bicep;
	hand.rotation = Math::Quaternion::RotationAxis(Math::Vector3::ZAxis(), bendWrist);
	hand.translation = Math::Vector3(0.0f, 3.0f, 0.0f);
	hand.parent = &forearm;
	hand.GenerateTransform();
	kBones.push_back(&bicep);
	kBones.push_back(&forearm);
	kBones.push_back(&hand);

	worldMatrix = Math::Matrix4::Identity();
	Math::Matrix4 viewMatrix = mCamera.GetViewMatrix(mCameraTransform);
	Math::Matrix4 projectionMatrix = mCamera.GetProjectionMatrix(Graphics::GraphicsSystem::Get()->GetAspectRatio());


	/*Math::Matrix4x4 rotateZ = Math::Matrix4x4::RotateZ(mTimer.GetTotalTime() * 1.0f);
	worldMatrix *= rotateZ;*/
	std::vector<Math::Matrix4> boneMatrices = GetWorldTransforms(kBones);
	ConstantData data;
	for (size_t i = 0; i < kBones.size(); ++i)
	{
		worldMatrix = boneMatrices[i] /** Math::Matrix4::Scaling(0.8f) * worldMatrix*/;
		data.wvp = Math::Transpose(worldMatrix * viewMatrix * projectionMatrix);
		mConstantBuffer.Set(data);
		mConstantBuffer.BindVS(); //matrix to vertex shader

		mVertexShader.Bind();
		mPixelShader.Bind();
		mMeshBuffer.Render();
	}

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

	Graphics::SimpleDraw::Flush(viewMatrix * projectionMatrix);

	Graphics::GraphicsSystem::Get()->EndRender();
}