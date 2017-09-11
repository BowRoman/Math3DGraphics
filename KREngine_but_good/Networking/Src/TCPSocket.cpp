#include "Precompiled.h"
#include "NetworkUtil.h"

#include "TCPSocket.h"
#include "SocketAddress.h"

Network::TCPSocket::TCPSocket()
	:mSocket(INVALID_SOCKET)
{
}

Network::TCPSocket::~TCPSocket()
{
	Terminate();
}

void Network::TCPSocket::Initialize()
{
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ASSERT(mSocket != INVALID_SOCKET, "Failed to create socket");
}

void Network::TCPSocket::Terminate()
{
	closesocket(mSocket);
}

int Network::TCPSocket::Connect(const SocketAddress& toAddress)
{
	return connect(mSocket, (LPSOCKADDR)&toAddress.mSockAddr, (int)sizeof(toAddress.mSockAddr));
}

int Network::TCPSocket::Bind(const SocketAddress& toAddress)
{
	return bind(mSocket, (LPSOCKADDR)&toAddress.mSockAddr, (int)sizeof(toAddress.mSockAddr));
}

int Network::TCPSocket::Listen(int backLog)
{
	return listen(mSocket, backLog);
}

Network::TCPSocket* Network::TCPSocket::Accept(SocketAddress& fromAddress)
{
	TCPSocket* newSocket = new TCPSocket();
	int addSize = (int)sizeof(fromAddress.mSockAddr);
	newSocket->mSocket = accept(mSocket, (LPSOCKADDR)&fromAddress.mSockAddr, &addSize);
	//ASSERT(newSocket.mSocket == INVALID_SOCKET, "client accept fail");
	return newSocket;
}

int Network::TCPSocket::Send(const void* buffer, int len)
{
	return send(mSocket, static_cast<const char*>(buffer), len, 0);
}

int Network::TCPSocket::Receive(void* buffer, int len)
{
	return recv(mSocket, static_cast<char*>(buffer), len, 0);
}
