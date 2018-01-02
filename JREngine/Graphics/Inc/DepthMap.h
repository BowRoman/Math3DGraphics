#pragma once

namespace Graphics
{

class DepthMap
{
private:
	DepthMap(const DepthMap&) = delete;
	DepthMap& operator=(const DepthMap&) = delete;

	ID3D11ShaderResourceView* mShaderResourceView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mViewport;
public:
	DepthMap();
	~DepthMap();

	void Initialize(uint32_t width, uint32_t height);
	void Terminate();

	void BeginRender();
	void EndRender();

	void BindPS(uint32_t index);
	void UnbindPS(uint32_t index);
}; // class DepthMap

} // namespace Graphics