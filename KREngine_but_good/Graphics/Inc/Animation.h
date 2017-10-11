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

	bool operator < (const Keyframe& rhs)
	{
		return time < rhs.time;
	}
}; // struct Keyframe

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
	// TODO: When implementing Play() ensure that the user has given a frame at time 0.0 
	Math::Matrix4 GetTransform(float time) const;

	void SortKeyframes();

	void AddKeyframe(Keyframe newFrame);
	void AddKeyframe(float keyTime, Math::Vector3 keyPos, Math::Vector3 keyScale, Math::Quaternion keyRot);
}; // class Animation

} // namespace Graphics