#ifndef INCLUDED_GRAPHICS_CONSTANTBUFFER_H
#define INCLUDED_GRAPHICS_CONSTANTBUFFER_H

namespace Graphics
{

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Initialize(uint32_t bufferSize, const void* initData = nullptr);
	void Terminate();

	void Set(const void* data);
	
	// BindVS: Binds set constant buffer to the given vertex shader slot. Defaults to 0
	void BindVS(uint32_t slot = 0);
	void BindGS(uint32_t slot = 0);
	// BindPS: Binds set constant buffer to the given pixel shader slot. Defaults to 0
	void BindPS(uint32_t slot = 0);
	void BindCS(uint32_t slot = 0);

private:
	ID3D11Buffer* mBuffer;
}; // class ConstantBuffer

template <typename T>
class TypedConstantBuffer : public ConstantBuffer
{
public:
	void Initialize()
	{
		ConstantBuffer::Initialize(GetPaddedSize());
	}

	void Initialize(const T& data)
	{
		ConstantBuffer::Initialize(GetPaddedSize(), &data);
	}

	void Set(const T& data)
	{
		ConstantBuffer::Set(&data);
	}

private:
	uint32_t GetPaddedSize() const
	{
		const uint32_t typeSize = sizeof(T);
		const uint32_t bufferSize = (typeSize % 16) ? ((typeSize / 16) + 1) * 16 : typeSize;
		return bufferSize;
	}
}; // class TypedConstantBuffer

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_CONSTANTBUFFER_H