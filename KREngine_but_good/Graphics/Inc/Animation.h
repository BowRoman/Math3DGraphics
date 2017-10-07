#pragma once

#include "Math\Inc\EngineMath.h"

namespace Graphics
{

	struct Keyframe
	{
		float time;
		Math::Vector3 position;
		Math::Vector3 scale;
		Math::Quaternion rotation;
	};

	class Animation
	{
	private:
		std::vector<Keyframe> mKeyframes;
		bool bLoop;
	public:
		Animation();
		~Animation();
		void Initialize(bool isLooped, uint32_t numKeyframes = 10);
		void Terminate();

		Math::Matrix4 GetTransform(float time) const;

		void AddKeyframe(Keyframe newFrame);
		void AddKeyframe(float keyTime, Math::Vector3 keyPos, Math::Vector3 keyScale, Math::Quaternion keyRot);
	};
}