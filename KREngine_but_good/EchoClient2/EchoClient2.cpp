#include "Networking\Inc\Network.h"

#define MAX_BUFFER_SIZE 1024


int main(int argc, char* argv[])
{
	system("color 02");
	const char* hostAddress = "127.0.0.1";
	uint16_t port = 8888;
	if (argc >= 2)
	{
		hostAddress = argv[1];
	}
	if (argc >= 3)
	{
		port = static_cast<uint16_t>(atoi(argv[1]));
	}

	if (!Network::Initialize())
	{
		printf("WSAStartup failed\n");
		return -1;
	}


	Network::TCPSocket tcpSocket;
	tcpSocket.Initialize();

	Network::SocketAddress mSocketAddress(hostAddress, port);

	int result = tcpSocket.Connect(mSocketAddress);
	if (result == SOCKET_ERROR)
	{
		printf("connect failed\n");
		return -1;
	}

	char receivedDataBuffer[MAX_BUFFER_SIZE];
	while (true)
	{
		std::string message;
		getline(std::cin, message);
		tcpSocket.Send(message.c_str(), message.length());
		int bytesReceived = tcpSocket.Receive(receivedDataBuffer, MAX_BUFFER_SIZE);
		if (bytesReceived == SOCKET_ERROR)
		{
			printf("recv failed \n");
			return -1;
		}
		if (bytesReceived == 0)
		{
			printf("Connection closed \n");
			break;
		}
		receivedDataBuffer[bytesReceived] = '\0';
		printf("Received: %s\n", receivedDataBuffer);
	}



	tcpSocket.Terminate();
	Network::Terminate();
	system("pause");
	return 0;
}