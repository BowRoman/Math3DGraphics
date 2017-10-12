#include "Precompiled.h"
#include "Animation.h"

#include <algorithm>

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
	ASSERT(time >= 0.0f, "[Animation] Error rendering. Time cannot be negative. (Time travel prohibited)");
	ASSERT(!mKeyframes.empty(), "[Animation] Error rendering. No Keyframes.");
	//ASSERT(time < mKeyframes.back().time, "[Animation] Given time exceeds keyframes.");

	if (bLoop)
	{
		time = std::fmodf(time, mKeyframes.back().time);
	}

	int startFrameIdx = 0;
	int endFrameIdx = 0;
	// iterate through vector to find the two closest keyframes to the given time
	for (; startFrameIdx < static_cast<int>(mKeyframes.size()) - 1; ++startFrameIdx)
	{
		if (mKeyframes[startFrameIdx].time <= time && mKeyframes[startFrameIdx + 1].time >= time)
		{
			endFrameIdx = startFrameIdx + 1;
			break;
		}
	}

	Math::Vector3 PosAtTime;
	Math::Vector3 ScaleAtTime;
	Math::Quaternion RotAtTime;

	// if theres only one frame
	// or
	// if the time extends beyond the last frame, use its data
	if (endFrameIdx == 0 || time >= mKeyframes.back().time)
	{
		PosAtTime = mKeyframes.back().position;
		ScaleAtTime = mKeyframes.back().scale;
		RotAtTime = mKeyframes.back().rotation;
	}
	// otherwise blend data based on the interpolant between the two keyframes
	else
	{
		const Keyframe& startFrame = mKeyframes[startFrameIdx];
		const Keyframe& endFrame = mKeyframes[endFrameIdx];
		float interpolant = (time - startFrame.time) / (endFrame.time - startFrame.time);

		PosAtTime = Math::Lerp(startFrame.position, endFrame.position, interpolant);
		ScaleAtTime = Math::Lerp(startFrame.scale, endFrame.scale, interpolant);
		RotAtTime = Math::Slerp(startFrame.rotation, endFrame.rotation, interpolant);
	}

	// construct a transform based on the chosen data
	return Math::Matrix4::Scaling(ScaleAtTime) * Math::Matrix4::RotationQuaternion(RotAtTime) * Math::Matrix4::Translation(PosAtTime);
}

void Graphics::Animation::SetLooping(bool loop)
{
	bLoop = loop;
}

// Sorts all Keyframes within mKeyframes based on ascending time variables
void Animation::SortKeyframes()
{
	// iterate through vector and sort based on time
	std::sort(mKeyframes.begin(), mKeyframes.end());
}

// Adds the given Keyframe to the mKeyframes vector and sorts it
void Animation::AddKeyframe(Keyframe newFrame)
{
	if (mKeyframes.empty())
	{
		ASSERT(newFrame.time == 0.0f, "[Animation] First added frame not set to time 0.0");
		mKeyframes.push_back(newFrame);
	}
	// search the vector to make sure a frame does not already exist with the same time as newFrame
	else
	{
		if (std::find_if(mKeyframes.begin(), mKeyframes.end(), [&newFrame](const Keyframe& val)
		{
			return (val.time == newFrame.time);
		}) != mKeyframes.end())
		{
			LOG("[Animation] Error: Frame already exists with given time.");
			return;
		}
		mKeyframes.push_back(newFrame);
		SortKeyframes();
	}
}

// Constructs a Keyframe from the given values and inserts it into the mKeyframes vector
void Animation::AddKeyframe(float keyTime, Math::Vector3 keyPos, Math::Vector3 keyScale, Math::Quaternion keyRot)
{
	AddKeyframe(Keyframe{ keyTime,keyPos,keyScale,keyRot });
}
