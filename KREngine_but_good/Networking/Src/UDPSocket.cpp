#include "Precompiled.h"
#include "NetworkUtil.h"

#include "UDPSocket.h"
#include "SocketAddress.h"

Network::UDPSocket::UDPSocket()
{
}

Network::UDPSocket::~UDPSocket()
{
	Terminate();
}

void Network::UDPSocket::Initialize()
{
	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	ASSERT(mSocket != INVALID_SOCKET, "Failed to create socket");
}

void Network::UDPSocket::Terminate()
{
	closesocket(mSocket);
}

int Network::UDPSocket::SetSocketOptions(const SocketAddress & toAddress)
{
	bool enabled = true;
	if (setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST, (char*)&enabled, sizeof(bool)) < 0)
	{
		printf("broadcast set up falied");
		return -1;
	}
	return 0;
}

int Network::UDPSocket::Bind(const SocketAddress & toAddress)
{
	return bind(mSocket, (SOCKADDR*)&toAddress.mSockAddr, (int)sizeof(toAddress.mSockAddr));
}

int Network::UDPSocket::SendTo(const void* buffer, int length, const SocketAddress& toAddress)
{
	return sendto(mSocket, static_cast<const char*>(buffer), length, 0, (SOCKADDR*)&toAddress.mSockAddr, (int)sizeof(toAddress.mSockAddr));
}

int Network::UDPSocket::ReceiveFrom(void* buffer, int length, const SocketAddress& fromAddress)
{
	int addSize = (int)sizeof(fromAddress.mSockAddr);
	return recvfrom(mSocket, static_cast<char*>(buffer), length, 0, (SOCKADDR*)&fromAddress.mSockAddr, &addSize);
}
