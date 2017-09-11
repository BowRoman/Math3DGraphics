#pragma once


namespace Network
{
	class LinkingContext
	{
		int mNextNetworkId;
		std::unordered_map<int, void*> mNetworkIdToInstanceMap;
		std::unordered_map<void*, int> mInstanceToNetworkIdMap;

	public:
		LinkingContext();
		~LinkingContext();

		int GetNetworkId(void* instance) const;
		void* GetInstance(int networkId) const;

		int Register(void* instance);
		int Register(void* instance, int networkId);
		void Unregister(void* instance);
	};
}