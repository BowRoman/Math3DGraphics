/*
File: AnimationClip.cpp
Author: Jake Roman-Barnes
*/
#include "Precompiled.h"
#include "AnimationClip.h"

using namespace Graphics;

AnimationClip::AnimationClip()
	: mDuration(0.0f)
	, mTicks(0.0f)
	, mTicksPerSecond(0.0f)
	, bPlaying(false)
{
}

AnimationClip::~AnimationClip()
{
}

void Graphics::AnimationClip::Play()
{
	bPlaying = true;
}

void Graphics::AnimationClip::Pause()
{
	bPlaying = false;
}

void Graphics::AnimationClip::Reset()
{
	mTicks = 0.0f;
}

void AnimationClip::Update(float deltaTime)
{
	if (bPlaying)
	{
		mTicks += mTicksPerSecond*deltaTime;
		// animations handle looping
	}
}

std::vector<Math::Matrix4> AnimationClip::GetTransforms()
{
	std::vector<Math::Matrix4> transforms;
	transforms.reserve(mBoneAnimations.size());
	for (auto& animation : mBoneAnimations)
	{
		transforms.push_back(animation.GetTransform(mTicks));
	}
	return transforms;
}
