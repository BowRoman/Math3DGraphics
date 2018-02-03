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
	sSoundManager = new SoundManager;

} // void SoundManager::StaticInitialize()

void SoundManager::StaticTerminate()
{
	ASSERT(nullptr != sSoundManager, "[SoundManager] Manager not active, cannot terminate.");
	delete sSoundManager;
	sSoundManager = nullptr;

} // void SoundManager::StaticTerminate()

SoundManager* SoundManager::Get()
{
	ASSERT(nullptr != sSoundManager, "[SoundManager] Manager not active, cannot get.");
	return sSoundManager;

} // SoundManager* SoundManager::Get()

SoundManager::SoundManager()
{
	mInstances.resize(4);

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

		auto effect = std::make_unique<DirectX::SoundEffect>(AudioSystem::Get()->mAudioEngine.get(), wbuffer);
		result.first->second = std::move(effect);
	}

	return hash;

} // SoundId SoundManager::Load(const char * filename)

void SoundManager::Clear()
{
	for (auto& inst : mInstances)
	{
		if (inst)
		{
			inst->Stop();
			inst.reset();
		}
	}
	for (auto& item : mInventory)
	{
		if (item.second)
		{
			item.second.reset();
		}
	}
	mInstances.clear();
	mInventory.clear();

} // void SoundManager::Clear()

// Calling this function will play the sound once.
// This does not create an instance, sound cannot be stopped or paused
void SoundManager::PlayEffect(SoundId id)
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		iter->second->Play();
	}

} // void SoundManager::PlayEffect(SoundId id)

InstanceId Audio::SoundManager::CreateInstance(SoundId id)
{
	auto iter = mInventory.find(id);
	ASSERT(iter != mInventory.end(), "[SoundManager] Not a valid SoundId.");
	auto effect = iter->second->CreateInstance();

	// Check for available indexes
	for (int i = 0; i < static_cast<int>(mInstances.max_size()); ++i)
	{
		if (!mInstances[i])
		{
			mInstances[i].swap(effect);
			effect.reset();

			return i;
		}
	}

	// No slots available
	mInstances.push_back(std::move(effect));
	return mInstances.size() - 1;

} // InstanceId Audio::SoundManager::CreateInstance(SoundId id)

void Audio::SoundManager::Play(InstanceId id, bool looping)
{
	if (mInstances[id]->GetState() == DirectX::SoundState::PAUSED)
	{
		mInstances[id]->Resume();
	}
	else
	{
		mInstances[id]->Play(looping);
	}

} // void Audio::SoundManager::Play(InstanceId id, bool looping)

void SoundManager::Stop(InstanceId id)
{
	mInstances[id]->Stop();

} // void SoundManager::Stop(SoundId id)

void SoundManager::Pause(InstanceId id)
{
	mInstances[id]->Pause();

} // void SoundManager::Pause(SoundId id)

bool Audio::SoundManager::IsPlaying(InstanceId id)
{
	return mInstances[id]->GetState() == DirectX::SoundState::PLAYING;
}

