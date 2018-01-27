#include "Precompiled.h"
#include "AudioSystem.h"

#include "External\DirectXTK\Inc\Audio.h"

using namespace Audio;

namespace
{
	AudioSystem* sAudioSystem = nullptr;

} // namespace

void AudioSystem::StaticInitialize()
{
	ASSERT(nullptr == sAudioSystem, "[AudioSystem] System not cleared, cannot be initialized.");

	sAudioSystem = new AudioSystem;
	sAudioSystem->Initialize();

} // void AudioSystem::StaticInitialize()

void AudioSystem::StaticTerminate()
{
	ASSERT(nullptr != sAudioSystem, "[AudioSystem] System not active, cannot terminate.");

	sAudioSystem->Terminate();
	SafeDelete(sAudioSystem);

} // void AudioSystem::StaticTerminate()

AudioSystem* AudioSystem::Get()
{
	ASSERT(nullptr != sAudioSystem, "[AudioSystem] System not active, cannot get.");

	return sAudioSystem;

} // AudioSystem* AudioSystem::Get()

AudioSystem::AudioSystem()
{

} // AudioSystem::AudioSystem()

AudioSystem::~AudioSystem()
{

} // AudioSystem::~AudioSystem()

void AudioSystem::Initialize()
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	DirectX::AUDIO_ENGINE_FLAGS eFlags = DirectX::AudioEngine_Default;
#ifdef _DEBUG
	eFlags = eFlags | DirectX::AudioEngine_Debug;
#endif
	mAudioEngine = std::make_unique<DirectX::AudioEngine>(eFlags);

} // void AudioSystem::Initialize()

void AudioSystem::Terminate()
{
	mAudioEngine.release();

} // void AudioSystem::Terminate()

void Audio::AudioSystem::Update()
{
	if (nullptr != mAudioEngine.get() && !mAudioEngine->Update())
	{
		if (mAudioEngine->IsCriticalError())
		{
			mAudioEngine.release();
		}
	}

} // void Audio::AudioSystem::Update()
