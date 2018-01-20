#pragma once

namespace Graphics
{

class RenderTarget
{
public:
	enum Format
	{
		RGBA_U8,
		RGBA_F16,
		RGBA_U32,
		R_F16,
		R_S32
	};

private:
	RenderTarget(const RenderTarget&) = delete;
	RenderTarget& operator=(const RenderTarget&) = delete;

	ID3D11ShaderResourceView* mShaderResourceView;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mViewport;

public:
	RenderTarget();
	~RenderTarget();

	void Initialize(uint32_t width, uint32_t height, Format format);
	void Terminate();

	void BeginRender();
	void EndRender();

	void BindPS(uint32_t index);
	void UnbindPS(uint32_t index);

}; // class RenderTarget

} // namespace Graphics