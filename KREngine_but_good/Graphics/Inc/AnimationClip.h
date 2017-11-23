/*
File: AnimationClip.h
Author: Jake Roman-Barnes
*/
#pragma once

#include "Animation.h"

namespace Graphics
{

class AnimationClip
{
public:
	AnimationClip();
	~AnimationClip();

	void Play();
	void Pause();
	void Reset();

	void Update(float deltaTime);

	std::vector<Math::Matrix4> GetTransforms();

private:
	friend class AnimatedModel;

	typedef std::vector<Animation> Animations;
	Animations mBoneAnimations;

	std::string mName;
	float mTicks;
	float mDuration;
	float mTicksPerSecond;
	bool bPlaying;
}; // class Animation

} // namespace Graphics