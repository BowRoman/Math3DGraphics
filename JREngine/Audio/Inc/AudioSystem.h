#pragma once

#include <memory>

namespace DirectX { class AudioEngine; }

namespace Audio
{

class AudioSystem
{
	AudioSystem(const AudioSystem&) = delete;
	AudioSystem& operator= (const AudioSystem&) = delete;

public:
	static void StaticInitialize();
	static void StaticTerminate();
	static AudioSystem* Get();

public:
	AudioSystem();
	~AudioSystem();

	void Initialize();
	void Terminate();

	std::unique_ptr<DirectX::AudioEngine> mAudioEngine;

}; // class AudioSystem

} // namespace Audio