#pragma once

#include "fmod.hpp"

namespace Audio
{

class Implementation
{
public:

	Implementation();
	~Implementation();

	void Update();

	FMOD::System* mpSystem;

	int mnNextChannelId;

	typedef std::map<std::string, FMOD::Sound*> SoundMap;
	typedef std::map<int, FMOD::Channel*> ChannelMap;

	SoundMap mSounds;
	ChannelMap mChannels;

}; // class Implementation

} // namespace Audio