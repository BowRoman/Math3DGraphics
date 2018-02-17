#include "Precompiled.h"

#include "AudioEngine.h"

namespace Audio
{

//-------------------------------------------[Implementation]-------------------------------------------

AudioEngineImpl::AudioEngineImpl()
	: mSystem{ nullptr }
	, mStudioSystem{ nullptr }
{

} // AudioEngineImpl::AudioEngineImpl()

AudioEngineImpl::~AudioEngineImpl()
{

} // AudioEngineImpl::~AudioEngineImpl()

void AudioEngineImpl::Initialize()
{
	// Create high-level system
	JRAudioEngine::ErrorCheck(FStudio::System::create(&mStudioSystem));
	JRAudioEngine::ErrorCheck(mStudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_VOL0_BECOMES_VIRTUAL, nullptr));

	// Grab low-level system
	JRAudioEngine::ErrorCheck(mStudioSystem->getLowLevelSystem(&mSystem));

} // void AudioEngineImpl::Initialize()

void AudioEngineImpl::Terminate()
{
	JRAudioEngine::ErrorCheck(mStudioSystem->unloadAll());
	JRAudioEngine::ErrorCheck(mStudioSystem->release());

} // void AudioEngineImpl::Terminate()

void AudioEngineImpl::Update()
{
	std::vector<ChannelMap::iterator> stoppedChannels;
	for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
	{
		bool bIsPlaying = false;
		it->second->isPlaying(&bIsPlaying);
		if (!bIsPlaying)
		{
			stoppedChannels.push_back(it);
		}
	}
	for (auto& it : stoppedChannels)
	{
		mChannels.erase(it);
	}
	JRAudioEngine::ErrorCheck(mStudioSystem->update());

} // void AudioEngineImpl::Update()

//-------------------------------------------[/Implementation]-------------------------------------------

//-------------------------------------------[Engine]-------------------------------------------

namespace
{
	JRAudioEngine* sJRAudioEngine = nullptr;
}

void JRAudioEngine::StaticInitialize()
{
	ASSERT(nullptr == sJRAudioEngine, "[AudioEngine] JRAudioEngine not cleared, cannot be initialized.");

	sJRAudioEngine = new JRAudioEngine;
	sJRAudioEngine->Initialize();

} // void JRAudioEngine::StaticInitialize()

void JRAudioEngine::StaticTerminate()
{
	ASSERT(nullptr != sJRAudioEngine, "[AudioEngine] JRAudioEngine not active, cannot terminate.");

	sJRAudioEngine->Terminate();
	SafeDelete(sJRAudioEngine);

} // void JRAudioEngine::StaticTerminate()

JRAudioEngine* JRAudioEngine::Get()
{
	ASSERT(nullptr != sJRAudioEngine, "[AudioEngine] JRAudioEngine not active, cannot get.");

	return sJRAudioEngine;

} // JRAudioEngine* JRAudioEngine::Get()


JRAudioEngine::JRAudioEngine()
{

} // JRAudioEngine::~JRAudioEngine()

JRAudioEngine::~JRAudioEngine()
{

} // JRAudioEngine::~JRAudioEngine()

void JRAudioEngine::Initialize()
{
	ASSERT(nullptr == mAudioEngineImpl, "[AudioEngine] mAudioEngineImpl not cleared, cannot be initialized.");
	mAudioEngineImpl = new AudioEngineImpl;
	mAudioEngineImpl->Initialize();

} // void JRAudioEngine::Initialize()

void JRAudioEngine::Terminate()
{
	ASSERT(nullptr != mAudioEngineImpl, "[AudioEngine] mAudioEngineImpl not cleared, cannot be initialized.");
	mAudioEngineImpl->Terminate();
	SafeDelete(mAudioEngineImpl);

} // void JRAudioEngine::Terminate()

void JRAudioEngine::Update()
{
	ASSERT(nullptr != mAudioEngineImpl, "[AudioEngine] mAudioEngineImpl not active, cannot update.");
	mAudioEngineImpl->Update();

} // void JRAudioEngine::Terminate()

void JRAudioEngine::ErrorCheck(FMOD_RESULT result)
{
	ASSERT(FMOD_OK == result, "[AudioEngine] ErrorCheck failed.");

} // int JRAudioEngine::ErrorCheck(FMOD_RESULT result)

uInt JRAudioEngine::LoadSound(const std::string& soundName, bool b3D, bool bLooping, bool bStream)
{
	std::string fullPath = mRoot + "/" + soundName;

	// create hash of file location
	std::hash<std::string> hasher;
	uInt hash = hasher(fullPath);

	// create place in map for sound pointer
	auto result = mAudioEngineImpl->mSounds.insert({ hash, nullptr });
	if (result.second)
	{
		// create sound mode
		FMOD_MODE eMode = FMOD_DEFAULT;
		eMode |= b3D ? FMOD_3D : FMOD_2D;
		eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

		// load sound through FMOD
		FMOD::Sound* sound = nullptr;
		JRAudioEngine::ErrorCheck(mAudioEngineImpl->mSystem->createSound(fullPath.c_str(), eMode, nullptr, &sound));

		// add sound pointer to map
		auto effect = std::make_unique<FMOD::Sound>(sound);
		result.first->second = std::move(effect);
	}

	return hash;

} // void JRAudioEngine::LoadSound(const std::string & soundName, bool b3D, bool bLooping, bool bStream)

void JRAudioEngine::UnloadSound(uInt soundHash)
{
	auto findIter = mAudioEngineImpl->mSounds.find(soundHash);
	if (findIter != mAudioEngineImpl->mSounds.end())
	{
		JRAudioEngine::ErrorCheck(findIter->second->release());
		mAudioEngineImpl->mSounds.erase(findIter);
	}

} // void JRAudioEngine::UnloadSound(const std::string & soundName)

void JRAudioEngine::LoadBank(const std::string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
{


} // void JRAudioEngine::LoadBank(const std::string & bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags)

void JRAudioEngine::LoadEvent(const std::string& eventName)
{

} // void JRAudioEngine::LoadEvent(const std::string & eventName)

int JRAudioEngine::PlayGivenSound(uInt soundHash, float volumeDB, const Math::Vector3& pos)
{
	int channelId = mAudioEngineImpl->mNextChannelId++;
	auto findIter = mAudioEngineImpl->mSounds.find(soundHash);
	ASSERT(findIter != mAudioEngineImpl->mSounds.end(), "[AudioEngine] Cannot play sound, hash not found.");

	FMOD::Channel* channel = nullptr;
	JRAudioEngine::ErrorCheck(mAudioEngineImpl->mSystem->playSound(findIter->second.get(), nullptr, true, &channel));
	if (nullptr != channel)
	{
		FMOD_MODE currMode;
		findIter->second->getMode(&currMode);
		if (currMode & FMOD_3D)
		{
			FMOD_VECTOR position = VectorToFmod(pos);
			JRAudioEngine::ErrorCheck(channel->set3DAttributes(&position, nullptr));
		}
		JRAudioEngine::ErrorCheck(channel->setVolume(dbToVolume(volumeDB)));
		JRAudioEngine::ErrorCheck(channel->setPaused(false));
		mAudioEngineImpl->mChannels[channelId] = channel;
	}
	return channelId;

} // void JRAudioEngine::PlayGivenSound(const std::string & soundName, const Math::Vector3 & pos, float volumeDB)

//-------------------------------------------[/Engine]-------------------------------------------


void JRAudioEngine::SetChannel3DPosition(int channelId, const Math::Vector3& pos)
{
	auto tFoundIt = mAudioEngineImpl->mChannels.find(channelId);
	if (tFoundIt == mAudioEngineImpl->mChannels.end())
		return;

	FMOD_VECTOR position = VectorToFmod(pos);
	JRAudioEngine::ErrorCheck(tFoundIt->second->set3DAttributes(&position, nullptr));

} // void JRAudioEngine::SetChannel3DPosition(int channelId, const Math::Vector3& pos)

void JRAudioEngine::SetChannelVolume(int channelId, float volumeDB)
{
	auto tFoundIt = mAudioEngineImpl->mChannels.find(channelId);
	if (tFoundIt != mAudioEngineImpl->mChannels.end())
	{
		JRAudioEngine::ErrorCheck(tFoundIt->second->setVolume(dbToVolume(volumeDB)));
	}

} // void JRAudioEngine::SetChannelVolume(int channelId, float volumeDB)

}