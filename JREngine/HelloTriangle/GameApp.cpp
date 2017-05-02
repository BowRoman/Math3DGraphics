#include "GameApp.h"

namespace
{
	// if this is altered, similar changes must be mae to vertex description and shader functions in the .fx file
	struct SimpleVertex
	{
		Math::Vector3 position;
		Math::Vector4 colour;
	};

	// vertex data
	const SimpleVertex kVertices[] =
	{
		{ Math::Vector3(-0.50f, +0.25f, 0.0f), Math::Vector4(Math::Vector4::Red()) },
		{ Math::Vector3(-0.25f, +0.50f, 0.0f), Math::Vector4(Math::Vector4::Orange()) },
		{ Math::Vector3(+0.00f, +0.25f, 0.0f), Math::Vector4(0.5f, 0.5f, 0.0f, 1.0f) },
		{ Math::Vector3(+0.25f, +0.50f, 0.0f), Math::Vector4(Math::Vector4::Lime()) },
		{ Math::Vector3(+0.50f, +0.25f, 0.0f), Math::Vector4(Math::Vector4::Green()) },
		{ Math::Vector3(+0.50f, -0.25f, 0.0f), Math::Vector4(Math::Vector4::Cyan()) },
		{ Math::Vector3(-0.50f, -0.25f, 0.0f), Math::Vector4(Math::Vector4::Magenta()) },
		{ Math::Vector3(-0.00f, -0.75f, 0.0f), Math::Vector4(Math::Vector4::Blue()) },
	};
	const int kVertexCount = sizeof(kVertices) / sizeof(kVertices[0]);

	// index data
	const uint32_t kIndices[]
	{
		0, 1, 2,
		2, 3, 4,
		0, 4, 5,
		0, 5, 6,
		6, 5, 7
	};
	const int kIndexCount = sizeof(kIndices) / sizeof(kIndices[0]);
}

GameApp::GameApp()
	: mVertexBuffer(nullptr)
	, mIndexBuffer(nullptr)
	, mInputLayout(nullptr)
	, mVertexShader(nullptr)
	, mPixelShader(nullptr)
{}

GameApp::~GameApp()
{}

void GameApp::OnInitialize(uint32_t width, uint32_t height)
{
	mWindow.Initialize(GetInstance(), GetAppName(), width, height);
	HookWindow(mWindow.GetWindowHandle());

	Graphics::GraphicsSystem::StaticInitialize(mWindow.GetWindowHandle(), false);
	Input::InputSystem::StaticInitialize(mWindow.GetWindowHandle());

	//create vertex buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * kVertexCount; // memory size in VRAM
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;


	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = kVertices;

	Graphics::GraphicsSystem::Get()->GetDevice()->CreateBuffer(&bd, &initData, &mVertexBuffer);

	//create index buffer
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(uint32_t) * kIndexCount; // memory size in VRAM
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;


	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = kIndices;

	Graphics::GraphicsSystem::Get()->GetDevice()->CreateBuffer(&bd, &initData, &mIndexBuffer);

	// create vertex shader
	// D3DCOMPILE_DEBUG flag improves shader debugging
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile
	(
		L"../Assets/Shaders/DoNothing.fx",
		nullptr,
		nullptr,
		"VS",
		"vs_5_0",
		shaderFlags,
		0,
		&shaderBlob,
		&errorBlob
	);
	ASSERT(SUCCEEDED(hr), "Failed to compile shader. Error: %s", (char*)errorBlob->GetBufferPointer());
	SafeRelease(errorBlob);

	// create vertex buffer
	Graphics::GraphicsSystem::Get()->GetDevice()->CreateVertexShader
	(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader
	);

	// define vertex description, create input layout using shader blob
	const D3D11_INPUT_ELEMENT_DESC kVertexLayout[] =
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	// create input layout
	Graphics::GraphicsSystem::Get()->GetDevice()->CreateInputLayout
	(
		kVertexLayout,
		sizeof(kVertexLayout) / sizeof(kVertexLayout[0]),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout
	);
	SafeRelease(shaderBlob);

	// create pixel shader
	hr = D3DCompileFromFile
	(
		L"../Assets/Shaders/DoNothing.fx",
		nullptr,
		nullptr,
		"PS",
		"ps_5_0",
		shaderFlags,
		0,
		&shaderBlob,
		&errorBlob
	);
	ASSERT(SUCCEEDED(hr), "Failed to compile shader. Error: %s", (char*)errorBlob->GetBufferPointer());
	SafeRelease(errorBlob);

	// create pixel buffer
	Graphics::GraphicsSystem::Get()->GetDevice()->CreatePixelShader
	(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader
	);
	SafeRelease(shaderBlob);
}

void GameApp::OnTerminate()
{
	SafeRelease(mPixelShader);
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);

	SafeRelease(mIndexBuffer);
	SafeRelease(mVertexBuffer);

	Input::InputSystem::StaticTerminate();
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

	Input::InputSystem* iS = Input::InputSystem::Get();
	iS->Update();

	if (iS->IsKeyPressed(Keys::ESCAPE))
	{
		PostQuitMessage(0);
	}

	Graphics::GraphicsSystem* gs = Graphics::GraphicsSystem::Get();

	gs->BeginRender(Math::Vector4::Black());
	// rendering

	// bind input layout, and vertex/pixel shaders
	ID3D11DeviceContext* context = gs->GetContext();
	context->IASetInputLayout(mInputLayout);
	context->VSSetShader(mVertexShader, nullptr, 0);
	context->PSSetShader(mPixelShader, nullptr, 0);

	// set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	// set index buffer
	context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//set primitive topology
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw mesh
	// using vertex buffer only
	// context->Draw(kVertexCount, 0);
	// using index buffer
	context->DrawIndexed(kIndexCount, 0, 0);

	gs->EndRender();
}