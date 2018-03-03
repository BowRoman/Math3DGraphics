#pragma once

#include <memory>

namespace Audio
{

class AudioEngineImpl
{
private:

	AudioEngineImpl() noexcept;
	~AudioEngineImpl();

	void Initialize();
	void Terminate();
	void Update();
	void Clear();

	uint32_t mNextChannelId;

	//-----------------------------------------------------------------------
	
	FMOD::System* mSystem;

	struct JRSound
	{
		std::unique_ptr<FMOD::Sound> sound;
		FMOD::ChannelGroup* channelGroup;
	};

	typedef std::unordered_map<SoundHandle, JRSound> SoundMap;
	typedef std::unordered_map<ChannelHandle, FMOD::Channel*> ChannelMap;
	typedef std::unordered_map<std::string, FMOD::ChannelGroup*> ChannelGroupMap;

	SoundMap mSounds;
	ChannelMap mChannels;
	ChannelGroupMap mChannelGroups;

	//-----------------------------------------------------------------------

	FMOD::Studio::System* mStudioSystem;

	typedef std::unordered_map<std::string, FMOD::Studio::Bank*> BankMap;
	typedef std::unordered_map<std::string, FMOD::Studio::EventInstance*> EventMap;

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
	JRAudioEngine* operator()() { return JRAudioEngine::Get(); }

public:
	JRAudioEngine() noexcept;
	~JRAudioEngine();

	void Initialize();
	void Terminate();
	void Update();
	static void ErrorCheck(FMOD_RESULT result);

	void SetRoot(const std::string& root) { mRoot = root; }

	SoundHandle LoadSound(const std::string& soundName, const std::string& ChannelGroupName = "", bool b3D = true, bool bLooping = false, bool bStream = false);
	void UnloadSound(SoundHandle soundHash);

	void LoadBank(const std::string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
	void LoadEvent(const std::string& eventName);

	void Set3DListenerAndOrientation(const Math::Vector3& pos = Math::Vector3{ 0, 0, 0 }, float volumeDB = 0.0f, const Math::Vector3& forward = Math::Vector3{ 0, 0, 0 }, const Math::Vector3& up = Math::Vector3{ 0, 0, 0 });

	ChannelHandle Play(SoundHandle soundHash, float volumeDB = 0.0f, const Math::Vector3& pos = Math::Vector3{ 0, 0, 0 }, float minDist = 1.0f, float maxDist = 10000.0f);
	void PlayEvent(const std::string& eventName);

	void CreateChannelGroup(const std::string& ChannelGroupName);
	FMOD::ChannelGroup* const GetChannelGroup(const std::string& ChannelGroupName) const;

	void StopChannel(ChannelHandle channelId);
	void StopEvent(const std::string& eventName, bool bImmediate = false);

	void GetEventParameter(const std::string& eventName, const std::string& parameterName, float* parameter);
	void SetEventParameter(const std::string& eventName, const std::string& parameterName, float value);
	void StopAllChannels();

	void SetChannel3DPosition(ChannelHandle channelId, const Math::Vector3& pos);
	void SetChannelVolume(ChannelHandle channelId, float volumeDB);

	bool IsPlaying(ChannelHandle channelId) const;
	bool IsEventPlaying(const std::string& eventName) const;

	float dbToVolume(float db) const { return powf(10.0f, 0.05f * db); }
	float VolumeTodb(float volume) const { return 20.0f * log10f(volume); }
	FMOD_VECTOR VectorToFmod(const Math::Vector3& pos);
};

} // namespace Audio