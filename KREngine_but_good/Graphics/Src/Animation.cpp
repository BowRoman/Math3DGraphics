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

// Returns the transform matrix for a given time frame
Math::Matrix4 Animation::GetTransform(float time) const
{
	ASSERT(time < mKeyframes.back().time, "[Animation] Given time exceeds keyframes.")
	ASSERT(mKeyframes.size() == 0, "[Animation] Error rendering. No Keyframes.");

	int startFrameIdx = 0;
	int endFrameIdx = 0;

	// iterate through vector to find the two closest keyframes to the given time
	for (; startFrameIdx < mKeyframes.size() - 1; ++startFrameIdx)
	{
		if (mKeyframes[startFrameIdx].time <= time && mKeyframes[startFrameIdx + 1].time >= time)
		{
			endFrameIdx = startFrameIdx + 1;
			break;
		}
	}

	// construct a transform based on the given time's interpolant between the two keyframes
	const Keyframe& startFrame = mKeyframes[startFrameIdx];
	const Keyframe& endFrame = mKeyframes[endFrameIdx];
	float interpolant = (time - startFrame.time) / (endFrame.time - startFrame.time);

	Math::Vector3 PosAtTime = Math::Lerp(startFrame.position, endFrame.position, interpolant);
	Math::Vector3 ScaleAtTime = Math::Lerp(startFrame.scale, endFrame.scale, interpolant);
	Math::Quaternion RotAtTime = Math::Slerp(startFrame.rotation, endFrame.rotation, interpolant);

	Math::Matrix4 scaleMat = Math::Matrix4::Scaling(ScaleAtTime);
	Math::Matrix4 rotMat = Math::Matrix4::RotationQuaternion(RotAtTime);
	Math::Matrix4 transMat = Math::Matrix4::Translation(PosAtTime);
	return scaleMat * rotMat * transMat;
}

// Sorts all Keyframes within mKeyframes based on ascending time variables
void Animation::SortKeyframes()
{
	// iterate through vector and sort based on time
	std::sort(mKeyframes.begin(), mKeyframes.end());
}

// Adds the given Keyframe to the mKeyframes vector
void Animation::AddKeyframe(Keyframe newFrame)
{
	// search the vector to make sure a frame does not already exist with the same time as newFrame
	if (std::find(mKeyframes.begin(), mKeyframes.end(), [&newFrame](const Keyframe& val)
	{
		return val.time == newFrame.time;
	}) != mKeyframes.end())
	{
		LOG("[Animation] Error: Frame already exists with given time.");
		return;
	}
	mKeyframes.push_back(newFrame);
	SortKeyframes();
}

// Constructs a Keyframe from the given values and inserts it into the mKeyframes vector
void Animation::AddKeyframe(float keyTime, Math::Vector3 keyPos, Math::Vector3 keyScale, Math::Quaternion keyRot)
{
	AddKeyframe(Keyframe{ keyTime,keyPos,keyScale,keyRot });
}
