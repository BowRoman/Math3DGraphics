#include "Precompiled.h"
#include "NetworkUtil.h"

using namespace Network;

bool Network::Initialize()
{
	WSAData wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result == 0)
	{
		return true;
	}
	return false;
}

void Network::Terminate()
{
	WSACleanup();
}
