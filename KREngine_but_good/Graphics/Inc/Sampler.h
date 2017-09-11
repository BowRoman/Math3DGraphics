#pragma once

namespace Graphics
{
	class Sampler
	{
		ID3D11SamplerState* mSamplerState;

	public:
		enum class Filter
		{
			Point, Linear, Anisotropic
		};

		enum class AddressMode
		{
			Border, Clamp, Mirror, Wrap
		};
		Sampler();
		~Sampler();

		void Initialize(Filter filter, AddressMode addressMode);
		void Terminate();

		void BindVS(uint32_t slot);
		void BindPS(uint32_t slot);

	private:

	};
}
