#include "Networking\Inc\Network.h"

#define MAX_BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
	system("color 03");
	uint16_t port = 8888;
	const char* hostAddress = "127.0.0.1";
	if (argc >= 2)
	{
		hostAddress = argv[1];
	}
	if (argc >= 3)
	{
		port = static_cast<uint16_t>(atoi(argv[1]));
	}
	Network::Initialize();


	Network::TCPSocket listener;
	listener.Initialize();
	
	Network::SocketAddress socketAddress(hostAddress, port);
	listener.Bind(socketAddress);
	listener.Listen(10);

	// accept client connection
	while (true)
	{
		
		Network::TCPSocket* client;
		client = listener.Accept(socketAddress);

		printf("Client connected\n");

		char buffer[MAX_BUFFER_SIZE];
		int bytesReceived = client->Receive(buffer, MAX_BUFFER_SIZE);
		if (bytesReceived == SOCKET_ERROR)
		{
			printf("recv failed \n");
			return -1;
		}
		if (bytesReceived == 0)
		{
			printf("Connection closed \n");
		}
		else
		{
			buffer[bytesReceived] = '\0';
			printf("Received: %s\n", buffer);

			int bytesSent = client->Send(buffer, bytesReceived);
			if (bytesSent == SOCKET_ERROR)
			{
				printf("send failed.\n");
				return -1;
			}
		}
		client->Terminate();
		SafeDelete(client);
	}


	listener.Terminate();
	Network::Terminate();
	return 0;
}