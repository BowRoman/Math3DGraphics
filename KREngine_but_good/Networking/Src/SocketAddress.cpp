#include "Precompiled.h"
#include "NetworkUtil.h"
#include "SocketAddress.h"

using namespace Network;

//----------------------------------------------------------------------------------------------------

Network::SocketAddress::SocketAddress()
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetIP4() = INADDR_ANY;
	GetPort() = 0;
}

//----------------------------------------------------------------------------------------------------

Network::SocketAddress::SocketAddress(const char* host, uint16_t port)
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetPort() = htons(port);
	inet_pton(AF_INET, host, &GetIP4());
}

//----------------------------------------------------------------------------------------------------

Network::SocketAddress::SocketAddress(uint16_t port)
{
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetIP4() = INADDR_ANY;
	GetPort() = htons(port);
}

//----------------------------------------------------------------------------------------------------

Network::SocketAddress::SocketAddress(const sockaddr& sockAddr)
{
	memcpy(&mSockAddr, &sockAddr, sizeof(sockaddr));
}

//----------------------------------------------------------------------------------------------------

const char* Network::SocketAddress::GetHost() const
{
	return inet_ntoa(GetAsSockAddrIn()->sin_addr);
}