#pragma once

#include <memory>

namespace Audio
{

struct SoundDescription
{
	SoundHandle handle;
	float volumeDB = 50.0f;
	float minDist = 0.0f;
	float maxdist = 1000.0f;

	Math::Vector3 position = Math::Vector3{ 0, 0, 0 };
};

struct ChannelDescription
{
	float frequency;
	float lowPassGain;
	float doppler3D;
	float pitch;
	// TODO: Add reverb
	Math::Vector3 position = Math::Vector3{ 0, 0, 0 };
};

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

	//------------------------------------------[System]------------------------------------------

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

	//------------------------------------------[/System]------------------------------------------

	//-------------------------------------------[Studio]------------------------------------------

	/*
	FMOD::Studio::System* mStudioSystem;

	typedef std::unordered_map<std::string, FMOD::Studio::Bank*> BankMap;
	typedef std::unordered_map<std::string, FMOD::Studio::EventInstance*> EventMap;

	BankMap mBanks; // Stores event information
	EventMap mEvents;
	*/

	//-------------------------------------------[/Studio]------------------------------------------

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
	static void ErrorCheck( FMOD_RESULT result );

	void SetRoot( const std::string& root ) { mRoot = root; }

	SoundHandle LoadSound( const std::string& soundName, const std::string& ChannelGroupName = "", bool b3D = true, bool bLooping = false, bool bStream = false );
	void UnloadSound( SoundHandle soundHash );

	ChannelHandle PlaySounds( SoundDescription& soundDesc );
	ChannelHandle PlaySounds( SoundHandle soundHandle );

	void Set3DListenerAndOrientation( Math::Vector3& pos = Math::Vector3{ 0, 0, 0 }, float volumeDB = 0.0f, Math::Vector3& forward = Math::Vector3{ 0, 0, 0 }, Math::Vector3& up = Math::Vector3{ 0, 0, 0 } ) const;

	bool CreateChannelGroup( const std::string& ChannelGroupName, const std::string& parentGroupName = "" );
	FMOD::ChannelGroup* const GetChannelGroup( const std::string& ChannelGroupName ) const; // Not sure if this is needed
	void PlayChannel( ChannelHandle channelId );
	void PauseChannel( ChannelHandle channelId );
	void StopChannel( ChannelHandle channelId );

	void StopAllChannels();

	void GetChannelProperties( ChannelHandle channelId, ChannelDescription& channelDesc ) const;
	ChannelDescription GetChannelProperties( ChannelHandle channelId ) const;
	void SetChannelProperties( ChannelHandle channelId, ChannelDescription channelDesc );
	void SetChannel3DPosition( ChannelHandle channelId, Math::Vector3& pos );
	void SetChannelVolume( ChannelHandle channelId, float volumeDB );

	bool IsPlaying( ChannelHandle channelId ) const;

	float DBToVolume( float db ) const { return powf( 10.0f, 0.05f * db ); }
	float VolumeToDB( float volume ) const { return 20.0f * log10f( volume ); }
	FMOD_VECTOR VectorToFmod( Math::Vector3& pos ) const;
	Math::Vector3 FmodToVector( FMOD_VECTOR& pos ) const;

	//-------------------------------------------[Studio]-------------------------------------------

	/*
	void LoadBank(const std::string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
	void LoadEvent(const std::string& eventName);

	void PlayEvent(const std::string& eventName);
	void StopEvent(const std::string& eventName, bool bImmediate = false);

	void GetEventParameter(const std::string& eventName, const std::string& parameterName, float* parameter);
	void SetEventParameter(const std::string& eventName, const std::string& parameterName, float value);

	bool IsEventPlaying(const std::string& eventName) const;
	*/
};

} // namespace Audio