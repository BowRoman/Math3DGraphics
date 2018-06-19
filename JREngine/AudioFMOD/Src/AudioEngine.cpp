#include "Precompiled.h"

#include "AudioEngine.h"

// All FMOD Studio functionality is commented out with the [STUDIO] tag. You're welcome, future me.

namespace Audio
{

//-------------------------------------------[Implementation]-------------------------------------------
namespace
{

//void InitializeStudio() [STUDIO]
//{
//	// Create high-level system
//	JRAudioEngine::ErrorCheck( FMOD::Studio::System::create( &mStudioSystem ) );
//	JRAudioEngine::ErrorCheck( mStudioSystem->initialize( 32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_VOL0_BECOMES_VIRTUAL, nullptr ) );
//
//	// Grab low-level system
//	JRAudioEngine::ErrorCheck( mStudioSystem->getLowLevelSystem( &mSystem ) );
//}
//
//void TerminateStudio()
//{
//	JRAudioEngine::ErrorCheck( mStudioSystem->unloadAll() );
//	JRAudioEngine::ErrorCheck( mStudioSystem->release() );
//}

}

AudioEngineImpl::AudioEngineImpl() noexcept
	: mSystem{ nullptr }
	//, mStudioSystem{ nullptr }
{

} // AudioEngineImpl::AudioEngineImpl()

AudioEngineImpl::~AudioEngineImpl()
{

} // AudioEngineImpl::~AudioEngineImpl()

void AudioEngineImpl::Initialize()
{
	//InitializeStudio(); [STUDIO]
	FMOD::System_Create(&mSystem);
	mSystem->init(32, FMOD_INIT_NORMAL, nullptr);

	FMOD::ChannelGroup *newChannelGroup = nullptr;

	JRAudioEngine::ErrorCheck(mSystem->createChannelGroup("Master", &newChannelGroup));

} // void AudioEngineImpl::Initialize()

void AudioEngineImpl::Terminate()
{
	//TerminateStudio(); [STUDIO]
	JRAudioEngine::ErrorCheck(mSystem->release());

} // void AudioEngineImpl::Terminate()

void AudioEngineImpl::Update()
{
	std::vector<ChannelMap::iterator> stoppedChannels;
	for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
	{
		bool bIsPlaying = false;
		it->second->isPlaying(&bIsPlaying); // or paused
		if (!bIsPlaying)
		{
			stoppedChannels.push_back(it);
		}
	}
	for (auto& it : stoppedChannels)
	{
		mChannels.erase(it);
	}
	//JRAudioEngine::ErrorCheck( mStudioSystem->update() ); [STUDIO]

} // void AudioEngineImpl::Update()

void AudioEngineImpl::Clear()
{
	/* [STUDIO]
	for( auto& events : mEvents )
	{
		if( nullptr != events.second )
		{
			events.second->stop( FMOD_STUDIO_STOP_IMMEDIATE );
			events.second->release();
		}
	}
	for( auto& bank : mBanks )
	{
		if( nullptr != bank.second )
		{
			bank.second->unload();
		}
	}
	*/
	for (auto& channel : mChannels)
	{
		if (nullptr != channel.second)
		{
			channel.second->stop();
		}
	}
	//mEvents.clear(); [STUDIO]
	//mBanks.clear(); [STUDIO]
	mSounds.clear();
	mChannels.clear();
}

//-------------------------------------------[/Implementation]-------------------------------------------

//-------------------------------------------[Engine]-------------------------------------------

namespace
{
JRAudioEngine* sJRAudioEngine = nullptr;
}

FMOD::Channel* JRAudioEngine::GetChannel(ChannelHandle channelId)
{
	auto findIter = mAudioEngineImpl->mChannels.find(channelId);
	if (mAudioEngineImpl->mChannels.end() != findIter)
	{
		return findIter->second;
	}
	return nullptr;
}

FMOD::Channel* JRAudioEngine::GetChannel(ChannelHandle channelId) const
{
	auto findIter = mAudioEngineImpl->mChannels.find(channelId);
	if (mAudioEngineImpl->mChannels.end() != findIter)
	{
		return findIter->second;
	}
	return nullptr;
}

void JRAudioEngine::StaticInitialize()
{
	ASSERT(nullptr == sJRAudioEngine, "[AudioEngine] JRAudioEngine not cleared, cannot be initialized.");

	sJRAudioEngine = new JRAudioEngine;
	sJRAudioEngine->Initialize();

} // void StaticInitialize()

void JRAudioEngine::StaticTerminate()
{
	ASSERT(nullptr != sJRAudioEngine, "[AudioEngine] JRAudioEngine not active, cannot terminate.");

	sJRAudioEngine->Terminate();
	SafeDelete(sJRAudioEngine);

} // void StaticTerminate()

JRAudioEngine* JRAudioEngine::Get()
{
	ASSERT(nullptr != sJRAudioEngine, "[AudioEngine] JRAudioEngine not active, cannot get.");

	return sJRAudioEngine;

} // JRAudioEngine* Get()


JRAudioEngine::JRAudioEngine() noexcept
	: mAudioEngineImpl{ nullptr }
{

} // JRAudioEngine() noexcept

JRAudioEngine::~JRAudioEngine()
{

} // ~JRAudioEngine()

void JRAudioEngine::Initialize()
{
	ASSERT(nullptr == mAudioEngineImpl, "[AudioEngine] mAudioEngineImpl not cleared, cannot be initialized.");
	mAudioEngineImpl = new AudioEngineImpl;
	mAudioEngineImpl->Initialize();

} // void Initialize()

void JRAudioEngine::Terminate()
{
	ASSERT(nullptr != mAudioEngineImpl, "[AudioEngine] mAudioEngineImpl not cleared, cannot be initialized.");
	mAudioEngineImpl->Terminate();

} // void Terminate()

void JRAudioEngine::Update()
{
	ASSERT(nullptr != mAudioEngineImpl, "[AudioEngine] mAudioEngineImpl not active, cannot update.");
	mAudioEngineImpl->Update();

} // void Terminate()

void JRAudioEngine::ErrorCheck(FMOD_RESULT result)
{
	ASSERT(FMOD_OK == result, "[AudioEngine] ErrorCheck failed.");

} // void ErrorCheck(FMOD_RESULT result)

// Loads a sound into inventory by filename. For ease of use, file root level should be set through SetRoot()
SoundHandle JRAudioEngine::LoadSound(const std::string& soundName, const std::string& ChannelGroupName, bool b3D, bool bLooping, bool bStream)
{
	std::string fullPath = mRoot + "/" + soundName;

	// create hash of file location
	std::hash<std::string> hasher;
	SoundHandle hash = hasher(fullPath);

	// create channel group
	FMOD::ChannelGroup *channelgroup;
	ErrorCheck(mAudioEngineImpl->mSystem->getMasterChannelGroup(&channelgroup));
	if (ChannelGroupName.length() > 0)
	{
		channelgroup = mAudioEngineImpl->mChannelGroups[ChannelGroupName];
	}

	// create place in map for sound pointer
	auto result = mAudioEngineImpl->mSounds.insert({ hash, AudioEngineImpl::JRSound{ nullptr, channelgroup } });
	if (result.second)
	{
		// create sound mode
		FMOD_MODE eMode = FMOD_DEFAULT;
		eMode |= b3D ? FMOD_3D : FMOD_2D;
		eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

		// load sound through FMOD
		FMOD::Sound* sound = nullptr;
		ErrorCheck(mAudioEngineImpl->mSystem->createSound(fullPath.c_str(), eMode, nullptr, &sound));

		// add sound pointer to map
		auto effect = std::unique_ptr<FMOD::Sound>(std::move(sound));
		result.first->second.sound = std::move(effect);
	}

	return hash;

} // SoundHandle LoadSound(const std::string& soundName, const std::string& ChannelGroupName, bool b3D, bool bLooping, bool bStream)

// Remove specified sound from inventory
void JRAudioEngine::UnloadSound(SoundHandle soundHash)
{
	auto findIter = mAudioEngineImpl->mSounds.find(soundHash);
	if (mAudioEngineImpl->mSounds.end() != findIter)
	{
		ErrorCheck(findIter->second.sound->release());
		mAudioEngineImpl->mSounds.erase(findIter);
	}

} // void UnloadSound(const std::string & soundName)

void JRAudioEngine::Set3DListenerAndOrientation(Math::Vector3& pos, float volumeDB, Math::Vector3& forward, Math::Vector3& up) const
{
	FMOD_VECTOR fPos{ VectorToFmod(pos) };
	FMOD_VECTOR fForward{ VectorToFmod(forward) };
	FMOD_VECTOR fUp{ VectorToFmod(up) };
	ErrorCheck(mAudioEngineImpl->mSystem->set3DListenerAttributes(0, &fPos, nullptr, &fForward, &fUp));

} // void Set3DListenerAndOrientation(Math::Vector3& pos, float volumeDB, Math::Vector3& forward, Math::Vector3& up) const

// Finds and plays the specified sound if it exists. Sound position will only affect sounds created as 3D
// Returns ID of the channel the sound is on
ChannelHandle JRAudioEngine::PlaySounds(SoundDescription& soundDesc)
{
	ChannelHandle channelId = mAudioEngineImpl->mNextChannelId++;
	auto findIter = mAudioEngineImpl->mSounds.find(soundDesc.handle);
	ASSERT(findIter != mAudioEngineImpl->mSounds.end(), "[AudioEngine] Error playing sound, hash not found.");

	FMOD::Channel* channel = nullptr;
	ErrorCheck(mAudioEngineImpl->mSystem->playSound(findIter->second.sound.get(), findIter->second.channelGroup, true, &channel));
	if (nullptr != channel)
	{
		FMOD_MODE currMode;
		findIter->second.sound->getMode(&currMode);

		// Set channel position if sound is 3D
		if (currMode & FMOD_3D)
		{
			FMOD_VECTOR position = VectorToFmod(soundDesc.position);
			ErrorCheck(channel->set3DAttributes(&position, nullptr));
			ErrorCheck(channel->set3DMinMaxDistance(soundDesc.minDist, soundDesc.maxdist));
			ErrorCheck(channel->set3DDistanceFilter(false, 0.5f, 1500.0f));
		}

		ErrorCheck(channel->setVolume(DBToVolume(soundDesc.volumeDB)));
		ErrorCheck(channel->setPaused(false));
		mAudioEngineImpl->mChannels[channelId] = channel;
		return channelId;
	}
	return -1;

} // ChannelHandle PlaySounds(SoundDescription& soundDesc)

ChannelHandle JRAudioEngine::PlaySounds(SoundHandle soundHandle)
{
	SoundDescription desc;
	desc.handle = soundHandle;
	return PlaySounds(desc);

} // ChannelHandle PlaySounds(SoundHandle soundHandle)

// Returns true if channel group was created successfully
bool JRAudioEngine::CreateChannelGroup(const std::string& ChannelGroupName, const std::string& parentGroupName)
{
	auto& groups = mAudioEngineImpl->mChannelGroups;
	auto iter = groups.find(ChannelGroupName);
	if (iter != groups.end())
	{
		return false;
	}

	ErrorCheck(mAudioEngineImpl->mSystem->createChannelGroup(ChannelGroupName.c_str(), &groups[ChannelGroupName]));

	if (parentGroupName.length() > 0)
	{
		groups[parentGroupName]->addGroup(groups[ChannelGroupName]);
	}

	return true;

} // bool CreateChannelGroup(const std::string& ChannelGroupName, const std::string& parentGroupName)

FMOD::ChannelGroup* const JRAudioEngine::GetChannelGroup(const std::string& ChannelGroupName) const
{
	return mAudioEngineImpl->mChannelGroups[ChannelGroupName];

} // FMOD::ChannelGroup* const GetChannelGroup(const std::string& ChannelGroupName) const

void JRAudioEngine::PlayChannel(ChannelHandle channelId)
{
	auto channel{ GetChannel(channelId) };
	if (nullptr != channel)
	{
		ErrorCheck(channel->setPaused(false));
	}

} // void PlayChannel(ChannelHandle channelId)

void JRAudioEngine::PauseChannel(ChannelHandle channelId)
{
	auto channel{ GetChannel(channelId) };
	if (nullptr != channel)
	{
		ErrorCheck(channel->setPaused(true));
	}

} // void PauseChannel(ChannelHandle channelId)

void JRAudioEngine::StopChannel(ChannelHandle channelId)
{
	auto channel{ GetChannel(channelId) };
	if (nullptr != channel)
	{
		ErrorCheck(channel->stop());
	}

} // void StopChannel(int channelId)

void JRAudioEngine::TogglePaused(ChannelHandle channelId)
{
	if (IsPaused(channelId))
	{
		PlayChannel(channelId);
	}
	else
	{
		PauseChannel(channelId);
	}

} // void TogglePaused(ChannelHandle channelId)

void JRAudioEngine::StopAllChannels()
{
	for (auto channel : mAudioEngineImpl->mChannels)
	{
		if (nullptr != channel.second)
		{
			channel.second->stop();
		}
	}

} // void StopAllChannels()

void JRAudioEngine::GetChannelProperties(ChannelHandle channelId, ChannelDescription& channelDesc) const
{
	auto channel{ GetChannel(channelId) };
	if (nullptr != channel)
	{
		channel->getFrequency(&channelDesc.frequency);
		channel->getLowPassGain(&channelDesc.lowPassGain);
		channel->get3DDopplerLevel(&channelDesc.doppler3D);
		channel->getPitch(&channelDesc.pitch);

		FMOD_VECTOR pos;
		FMOD_VECTOR vel;
		channel->get3DAttributes(&pos, &vel);
		channelDesc.position = FmodToVector(pos);
	}

} // void GetChannelProperties(ChannelHandle channelId, ChannelDescription& channelDesc) const

ChannelDescription JRAudioEngine::GetChannelProperties(ChannelHandle channelId) const
{
	ChannelDescription desc;
	GetChannelProperties(channelId, desc);
	return desc;

} // ChannelDescription GetChannelProperties(ChannelHandle channelId) const

void JRAudioEngine::SetChannelProperties(ChannelHandle channelId, ChannelDescription channelDesc)
{
	auto channel{ GetChannel(channelId) };
	if (nullptr != channel)
	{
		channel->setFrequency(channelDesc.frequency);
		channel->setLowPassGain(channelDesc.lowPassGain);
		channel->set3DDopplerLevel(channelDesc.doppler3D);
		channel->setPitch(channelDesc.pitch);

		FMOD_VECTOR pos, vel;
		channel->get3DAttributes(&pos, &vel);
		channel->set3DAttributes(&VectorToFmod(channelDesc.position), &vel);
	}

} // void SetChannelProperties(ChannelHandle channelId, ChannelDescription channelDesc)

void JRAudioEngine::SetChannel3DPosition(ChannelHandle channelId, Math::Vector3& pos)
{
	auto channel{ GetChannel(channelId) };
	if (nullptr != channel)
	{
		FMOD_VECTOR position = VectorToFmod(pos);
		ErrorCheck(channel->set3DAttributes(&position, nullptr));
	}

} // void SetChannel3DPosition(sizet channelId, const Math::Vector3& pos)

void JRAudioEngine::SetChannelVolume(ChannelHandle channelId, float volumeDB)
{
	auto channel{ GetChannel(channelId) };
	if (nullptr != channel)
	{
		ErrorCheck(channel->setVolume(DBToVolume(volumeDB)));
	}

} // void SetChannelVolume(ChannelHandle channelId, float volumeDB)

bool JRAudioEngine::IsPlaying(ChannelHandle channelId) const
{
	bool playing = false;
	// Note: isPlaying returns true for paused sounds
	ErrorCheck(mAudioEngineImpl->mChannels[channelId]->isPlaying(&playing));
	return playing;

} // bool IsPlaying(ChannelHandle channelId) const

bool JRAudioEngine::IsPaused(ChannelHandle channelId) const
{
	bool paused = false;
	ErrorCheck(mAudioEngineImpl->mChannels[channelId]->getPaused(&paused));
	return paused;

} // bool IsPaused(ChannelHandle channelId) const

FMOD_VECTOR JRAudioEngine::VectorToFmod(Math::Vector3& pos) const
{
	FMOD_VECTOR fVec;
	fVec.x = pos.x;
	fVec.y = pos.y;
	fVec.z = pos.z;
	return fVec;

} // FMOD_VECTOR VectorToFmod(const Math::Vector3& pos)

Math::Vector3 JRAudioEngine::FmodToVector(FMOD_VECTOR& pos) const
{
	Math::Vector3 vec;
	vec.x = pos.x;
	vec.y = pos.y;
	vec.z = pos.z;
	return vec;

} // Math::Vector3 FmodToVector(FMOD_VECTOR& pos) const

//-------------------------------------------[Studio]-------------------------------------------

/*
void JRAudioEngine::LoadBank(const std::string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
{
	auto foundIter = mAudioEngineImpl->mBanks.find(bankName);
	if (mAudioEngineImpl->mBanks.end() == foundIter)
	{
		FMOD::Studio::Bank* bank = nullptr;
		ErrorCheck(mAudioEngineImpl->mStudioSystem->loadBankFile(bankName.c_str(), flags, &bank));
		if (nullptr != bank)
		{
			mAudioEngineImpl->mBanks[bankName] = bank;
		}
	}

} // void JRAudioEngine::LoadBank(const std::string & bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags)

void JRAudioEngine::LoadEvent(const std::string& eventName)
{
	auto foundIter = mAudioEngineImpl->mEvents.find(eventName);
	if (mAudioEngineImpl->mEvents.end() == foundIter)
	{
		FMOD::Studio::EventDescription* eventDesc = nullptr;
		ErrorCheck(mAudioEngineImpl->mStudioSystem->getEvent(eventName.c_str(), &eventDesc));
		if (nullptr != eventDesc)
		{
			FMOD::Studio::EventInstance* eventInst = nullptr;
			ErrorCheck(eventDesc->createInstance(&eventInst));
			if (nullptr != eventInst)
			{
				mAudioEngineImpl->mEvents[eventName] = eventInst;
			}
		}
	}

} // void JRAudioEngine::LoadEvent(const std::string & eventName)

void JRAudioEngine::PlayEvent(const std::string& eventName)
{
	auto findIter = mAudioEngineImpl->mEvents.find(eventName);
	ASSERT(mAudioEngineImpl->mEvents.end() != findIter, "[AudioEngine] Error playing event. Event not found.");

	findIter->second->start();

} // void JRAudioEngine::PlayEvent(const std::string& eventName)

void JRAudioEngine::StopEvent(const std::string& eventName, bool bImmediate)
{
	auto findIter = mAudioEngineImpl->mEvents.find(eventName);
	if (mAudioEngineImpl->mEvents.end() != findIter)
	{
		FMOD_STUDIO_STOP_MODE eMode;
		eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
		ErrorCheck(findIter->second->stop(eMode));
	}

} // void JRAudioEngine::StopEvent(const std::string& eventName, bool bImmediate)

void JRAudioEngine::GetEventParameter(const std::string& eventName, const std::string& parameterName, float* parameter)
{
	auto findIter = mAudioEngineImpl->mEvents.find(eventName);
	if (mAudioEngineImpl->mEvents.end() != findIter)
	{
		FMOD::Studio::ParameterInstance* parameterInst = nullptr;
		ErrorCheck(findIter->second->getParameter(parameterName.c_str(), &parameterInst));
		ErrorCheck(parameterInst->getValue(parameter));
	}

} // void JRAudioEngine::GetEventParameter(const std::string& eventName, const std::string& eventParameter, float* parameter)

void JRAudioEngine::SetEventParameter(const std::string& eventName, const std::string& parameterName, float value)
{
	auto findIter = mAudioEngineImpl->mEvents.find(eventName);
	if (mAudioEngineImpl->mEvents.end() != findIter)
	{
		FMOD::Studio::ParameterInstance* parameter = nullptr;
		ErrorCheck(findIter->second->getParameter(parameterName.c_str(), &parameter));
		ErrorCheck(parameter->setValue(value));
	}

} // void JRAudioEngine::SetEventParameter(const std::string& eventName, const std::string& parameterName, float value)

bool JRAudioEngine::IsEventPlaying(const std::string& eventName) const
{
	auto findIter = mAudioEngineImpl->mEvents.find(eventName);
	if (mAudioEngineImpl->mEvents.end() != findIter)
	{
		FMOD_STUDIO_PLAYBACK_STATE* state = nullptr;
		return (FMOD_STUDIO_PLAYBACK_PLAYING == findIter->second->getPlaybackState(state));
	}
	return false;

} // bool JRAudioEngine::IsEventPlaying(const std::string& eventName) const
*/

//-------------------------------------------[/Studio]-------------------------------------------

//-------------------------------------------[/Engine]-------------------------------------------

}