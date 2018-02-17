#pragma once

namespace Audio
{

class Implementation
{
public:

	Implementation();
	~Implementation();

	void Update();

	uInt mNextChannelId;

	//-----------------------------------------------------------------------

	FMOD::System* mSystem;

	typedef std::map<std::string, FMOD::Sound*> SoundMap;
	typedef std::map<int, FMOD::Channel*> ChannelMap;

	SoundMap mSounds;
	ChannelMap mChannels;

	//-----------------------------------------------------------------------

	FStudio::System* mStudioSystem;

	typedef std::map<std::string, FStudio::Bank*> BankMap;
	typedef std::map<std::string, FStudio::EventInstance*> EventMap;

	BankMap mBanks;
	EventMap mEvents;

}; // class Implementation

class AudioEngine
{
public:
	static void Initialize();
	static void Update();
	static void Terminate();
	static int ErrorCheck(FMOD_RESULT result);

	void LoadBank(const std::string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
	void LoadEvent(const std::string& eventName);
	void LoadSound(const std::string& soundName, bool b3D = true, bool bLooping = false, bool bStream = false);

	void UnloadSound(const std::string& soundName);
	void Set3DListenerAndOrientation(const Math::Vector3& vPos = Math::Vector3{ 0, 0, 0 }, float volumeDB = 0.0f);

	void PlayGivenSound(const std::string& soundName, const Math::Vector3& pos = Math::Vector3{ 0, 0, 0 }, float volumeDB = 0.0f);
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