#include "Precompiled.h"
#include "LinkingContext.h"

using namespace Network;


Network::LinkingContext::LinkingContext()
	:mNextNetworkId(0)
{
}

Network::LinkingContext::~LinkingContext()
{
}

int Network::LinkingContext::GetNetworkId(void* instance) const
{
	auto iter = mInstanceToNetworkIdMap.find(instance);
	if (iter != mInstanceToNetworkIdMap.end())
	{
		return iter->second;
	}
	else
	{
		return -1;
	}
}

void* Network::LinkingContext::GetInstance(int networkId) const
{
	auto iter = mNetworkIdToInstanceMap.find(networkId);
	if (iter != mNetworkIdToInstanceMap.end())
	{
		return iter->second;
	}
	else
	{
		return nullptr;
	}
	
}

int Network::LinkingContext::Register(void* instance)
{
	ASSERT(GetNetworkId(instance) == -1, "[LinkingContext] Instance already registered");
	mInstanceToNetworkIdMap[instance] = mNextNetworkId;
	mNetworkIdToInstanceMap[mNextNetworkId] = instance;
	return mNextNetworkId++;
}

int Network::LinkingContext::Register(void* instance, int networkId)
{
	ASSERT(GetNetworkId(instance) == -1, "[LinkingContext] Instance already registered");
	ASSERT(GetInstance(networkId) == nullptr, "[LinkingContext] Instance already registered");
	mInstanceToNetworkIdMap[instance] = networkId;
	mNetworkIdToInstanceMap[networkId] = instance;
	mNextNetworkId = std::max(mNextNetworkId, networkId + 1);
	return mNextNetworkId;
}

void Network::LinkingContext::Unregister(void* instance)
{
	auto iter = mInstanceToNetworkIdMap.find(instance);
	if (iter != mInstanceToNetworkIdMap.end())
	{
		int networkId = iter->second;
		mInstanceToNetworkIdMap.erase(instance);
		mNetworkIdToInstanceMap.erase(networkId);
	}
}
