#ifndef INCLUDED_GRAPHICS_SAMPLER_H
#define INCLUDED_GRAPHICS_SAMPLER_H

namespace Graphics
{

class Sampler
{
public:
	enum class Filter { Point, Linear, Anisotropic };
	enum class AddressMode { Border, Clamp, Mirror, Wrap };

	Sampler();
	~Sampler();

	void Initialize(Filter filter, AddressMode mode);
	void Terminate();

	void BindVS(uint32_t slot);
	void BindPS(uint32_t slot);

private:
	ID3D11SamplerState *mSampler;
};

} // namespace Graphics

#endif // #ifndef INCLUDED_GRAPHICS_SAMPLER_H