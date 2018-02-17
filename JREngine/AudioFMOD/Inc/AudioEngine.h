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

	uInt mNextChannelId;

	//-----------------------------------------------------------------------

	FMOD::System* mSystem;

	typedef std::unordered_map<uInt, std::unique_ptr<FMOD::Sound>> SoundMap;
	typedef std::unordered_map<int, FMOD::Channel*> ChannelMap;

	SoundMap mSounds;
	ChannelMap mChannels;

	//-----------------------------------------------------------------------

	FStudio::System* mStudioSystem;

	typedef std::unordered_map<std::string, FStudio::Bank*> BankMap;
	typedef std::unordered_map<std::string, FStudio::EventInstance*> EventMap;

	BankMap mBanks;
	EventMap mEvents;

	friend class JRAudioEngine;

}; // class Implementation

class JRAudioEngine
{
private:
	AudioEngineImpl* mAudioEngineImpl;

public:
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

	uInt LoadSound(const std::string& soundName, bool b3D = true, bool bLooping = false, bool bStream = false);
	void UnloadSound(uInt soundHash);

	void LoadBank(const std::string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
	void LoadEvent(const std::string& eventName);

	void Set3DListenerAndOrientation(const Math::Vector3& vPos = Math::Vector3{ 0, 0, 0 }, float volumeDB = 0.0f);

	int PlayGivenSound(uInt soundHash, float volumeDB = 0.0f, const Math::Vector3& pos = Math::Vector3{ 0, 0, 0 });
	void PlayEvent(const std::string& eventName);

	void StopChannel(int channelId);
	void StopEvent(const std::string& eventName, bool bImmediate = false);

	void GetEventParameter(const std::string& eventName, const std::string& eventParameter, float* parameter);
	void SetEventParameter(const std::string& eventName, const std::string& parameterName, float value);
	void StopAllChannels();

	void SetChannel3DPosition(int channelId, const Math::Vector3& pos);
	void SetChannelVolume(int channelId, float volumeDB);

	bool IsPlaying(int channelId) const;
	bool IsEventPlaying(const std::string& eventName) const;

	float dbToVolume(float db);
	float VolumeTodb(float volume);
	FMOD_VECTOR VectorToFmod(const Math::Vector3& pos);
};

} // namespace Audio