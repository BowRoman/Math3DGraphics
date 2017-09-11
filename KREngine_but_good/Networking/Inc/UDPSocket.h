#pragma once

namespace Network
{
	class SocketAddress;
	class UDPSocket
	{
		SOCKET mSocket;
	public:
		UDPSocket();
		~UDPSocket();
		void Initialize();
		void Terminate();

		int SetSocketOptions(const SocketAddress& toAddress);
		int Bind(const SocketAddress& toAddress);
		int SendTo(const void* buffer, int length, const SocketAddress& toAddress);
		int ReceiveFrom(void* buffer, int length, const SocketAddress& fromAddress);
	private:

	};
}
