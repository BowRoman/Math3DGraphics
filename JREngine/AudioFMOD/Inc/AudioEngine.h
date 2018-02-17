#pragma once

#include <memory>

namespace Audio
{

class AudioEngineImpl
{
private:

	AudioEngineImpl();
	~AudioEngineImpl();

	void Initialize();
	void Terminate();
	void Update();
	void Clear();

	sizet mNextChannelId;

	//-----------------------------------------------------------------------

	FMOD::System* mSystem;

	typedef std::unordered_map<sizet, std::unique_ptr<FMOD::Sound>> SoundMap;
	typedef std::unordered_map<sizet, FMOD::Channel*> ChannelMap;

	SoundMap mSounds;
	ChannelMap mChannels;

	//-----------------------------------------------------------------------

	FStudio::System* mStudioSystem;

	typedef std::unordered_map<std::string, FStudio::Bank*> BankMap;
	typedef std::unordered_map<std::string, FStudio::EventInstance*> EventMap;

	BankMap mBanks; // Stores event information
	EventMap mEvents;

	friend class JRAudioEngine;

}; // class Implementation

class JRAudioEngine
{
private:
	AudioEngineImpl* mAudioEngineImpl;
	std::string mRoot;

public:
	static void StaticInitialize();
	static void StaticTerminate();
	static JRAudioEngine* Get();

public:
	JRAudioEngine();
	~JRAudioEngine();

	void Initialize();
	void Terminate();
	void Update();
	static void ErrorCheck(FMOD_RESULT result);

	void SetRoot(const std::string& root) { mRoot = root; }

	sizet LoadSound(const std::string& soundName, bool b3D = true, bool bLooping = false, bool bStream = false);
	void UnloadSound(sizet soundHash);

	void LoadBank(const std::string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
	void LoadEvent(const std::string& eventName);

	void Set3DListenerAndOrientation(const Math::Vector3& pos = Math::Vector3{ 0, 0, 0 }, float volumeDB = 0.0f, const Math::Vector3& forward = Math::Vector3{ 0, 0, 0 }, const Math::Vector3& up = Math::Vector3{ 0, 0, 0 });

	sizet Play(sizet soundHash, float volumeDB = 0.0f, const Math::Vector3& pos = Math::Vector3{ 0, 0, 0 }, float minDist = 1.0f, float maxDist = 10000.0f);
	void PlayEvent(const std::string& eventName);

	void StopChannel(int channelId);
	void StopEvent(const std::string& eventName, bool bImmediate = false);

	void GetEventParameter(const std::string& eventName, const std::string& parameterName, float* parameter);
	void SetEventParameter(const std::string& eventName, const std::string& parameterName, float value);
	void StopAllChannels();

	void SetChannel3DPosition(sizet channelId, const Math::Vector3& pos);
	void SetChannelVolume(sizet channelId, float volumeDB);

	bool IsPlaying(int channelId) const;
	bool IsEventPlaying(const std::string& eventName) const;

	float dbToVolume(float db) const { return powf(10.0f, 0.05f * db); }
	float VolumeTodb(float volume) const { return 20.0f * log10f(volume); }
	FMOD_VECTOR VectorToFmod(const Math::Vector3& pos);
};

} // namespace Audio