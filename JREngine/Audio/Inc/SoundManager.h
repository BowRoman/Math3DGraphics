#pragma once

#include <functional>
#include <memory>

namespace DirectX { class SoundEffect; class SoundEffectInstance; }

namespace Audio
{

class SoundManager
{
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator= (const SoundManager&) = delete;

public:
	static void StaticInitialize();
	static void StaticTerminate();
	static SoundManager* Get();

public:
	SoundManager();
	~SoundManager();

	void SetFilePath(const char* root);

	SoundId Load(const char* filename);
	void Clear();

	// Basic sound control
	void PlayEffect(SoundId id);

	void CreateInstance(SoundId id);
	void Play(InstanceId id, bool looping = false);
	void Stop(InstanceId id);
	void Pause(InstanceId id);

	// TODO: 3D sound control
	// ListenerId CreateListener(std::function<Math::Vector3()> fPosition);


private:
	std::string mRoot;

	std::unordered_map<SoundId, std::unique_ptr<DirectX::SoundEffect>> mInventory;
	std::unordered_map<InstanceId, std::unique_ptr<DirectX::SoundEffectInstance>> mInstances;

}; // class SoundManager

} // namespace Audio