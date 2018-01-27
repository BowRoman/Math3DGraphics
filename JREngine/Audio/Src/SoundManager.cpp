#include "Precompiled.h"
#include "SoundManager.h"

#include "AudioSystem.h"
#include <External/DirectXTK/Inc/Audio.h>

using namespace Audio;

namespace
{
	SoundManager* sSoundManager = nullptr;
}

void SoundManager::StaticInitialize()
{
	ASSERT(nullptr == sSoundManager, "[SoundManager] Manager not cleared, cannot initialize.");

} // void SoundManager::StaticInitialize()

void SoundManager::StaticTerminate()
{
	ASSERT(nullptr != sSoundManager, "[SoundManager] Manager not active, cannot terminate.");

} // void SoundManager::StaticTerminate()

SoundManager* SoundManager::Get()
{
	ASSERT(nullptr != sSoundManager, "[SoundManager] Manager not active, cannot get.");

} // SoundManager* SoundManager::Get()

SoundManager::SoundManager()
{

} // SoundManager::SoundManager()

SoundManager::~SoundManager()
{
	ASSERT(mInventory.empty(), "[SoundManager] Inventory must be cleared before destruction.");


} // SoundManager::~SoundManager()

void SoundManager::SetFilePath(const char* root)
{
	mRoot = root;
} // void SoundManager::SetFilePath(const char * root)

SoundId SoundManager::Load(const char* filename)
{
	std::string fullPath = mRoot + "/" + filename;

	std::hash<std::string> hasher;
	SoundId hash = hasher(fullPath);

	auto result = mInventory.insert({ hash, nullptr });
	if (result.second)
	{
		wchar_t wbuffer[1024];
		mbstowcs_s(nullptr, wbuffer, fullPath.c_str(), 1024);

		auto effect = std::make_unique<DirectX::SoundEffect>(AudioSystem::Get()->mAudioEngine, wbuffer);
		result.first->second = std::move(effect);
	}

	return hash;
} // SoundId SoundManager::Load(const char * filename)

void SoundManager::Clear()
{
	for (auto& item : mInstances)
	{
		if (item.second)
		{
			item.second->Stop();
			item.second.reset();
		}
	}
	for (auto& inst : mInventory)
	{
		if (inst.second)
		{
			inst.second.reset();
		}
	}
	mInstances.clear();
	mInventory.clear();

} // void SoundManager::Clear()

// Calling this function will play the sound once.
// This does not create an instance, sound cannot be stopped or paused
void SoundManager::PlayEffect(SoundId id)
{
	auto iter = mInstances.find(id);
	if (iter != mInstances.end())
	{
		iter->second->Play();
	}

} // void SoundManager::PlayEffect(SoundId id)

void Audio::SoundManager::CreateInstance(SoundId id)
{
}

void Audio::SoundManager::Play(InstanceId id, bool looping)
{
}

void SoundManager::Stop(InstanceId id)
{
	auto iter = mInstances.find(id);
	if (iter != mInstances.end())
	{
		iter->second->Stop();
	}
} // void SoundManager::Stop(SoundId id)

void SoundManager::Pause(InstanceId id)
{
	auto iter = mInstances.find(id);
	if (iter != mInstances.end())
	{
		iter->second->Pause();
	}
} // void SoundManager::Pause(SoundId id)

