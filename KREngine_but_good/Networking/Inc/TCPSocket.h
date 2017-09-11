#pragma once



namespace Network
{
	class SocketAddress;
	class TCPSocket
	{
		SOCKET mSocket;
	public:
		TCPSocket();
		~TCPSocket();
		void Initialize();
		void Terminate();

		int Connect(const SocketAddress& toAddress);
		int Bind(const SocketAddress& toAddress);
		int Listen(int backLog);
		TCPSocket* Accept(SocketAddress& fromAddress);
		int Send(const void* buffer, int len);
		int Receive(void* buffer, int len);

	private:
		TCPSocket(const TCPSocket&) = delete;
		
	};
}
