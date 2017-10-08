#include "Precompiled.h"
#include "Animation.h"

using namespace Graphics;

Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::Initialize(bool isLooped, uint32_t numKeyframes)
{
	bLoop = isLooped;
	mKeyframes.reserve(numKeyframes);
}

void Animation::Terminate()
{
	mKeyframes.clear();
}

Math::Matrix4 Graphics::Animation::GetTransform(float time) const
{
	ASSERT(mKeyframes.size() == 0, "[Animation] Error rendering. No Keyframes.");

	return Math::Matrix4::Identity();
}

void Animation::AddKeyframe(Keyframe newFrame)
{
	mKeyframes.push_back(newFrame);

	// iterate through vector and sort based on time
}

void Graphics::Animation::AddKeyframe(float keyTime, Math::Vector3 keyPos, Math::Vector3 keyScale, Math::Quaternion keyRot)
{
	mKeyframes.push_back(Keyframe{ keyTime,keyPos,keyScale,keyRot });
}
