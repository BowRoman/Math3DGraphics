#define _WINSOCK_DEPRICATED_NO_WARNINGS


#include <iostream>
#include <cstdint>
#include <cstdio>
#include <string>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER_SIZE 1024



int main(int argc, char* argv[])
{
	system("color 03");
	uint16_t port = 8888;

	if (argc >= 2)
	{
		port = static_cast<uint16_t>(atoi(argv[1]));
	}

	// Init
	WSAData wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("WSAStartup failed\n");
		return -1;
	}

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listener == INVALID_SOCKET)
	{
		printf("Failed to create socket\n");
		return -1;
	}

	SOCKADDR_IN serverInfo;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;
	serverInfo.sin_port = htons(port);

	result = bind(listener, (LPSOCKADDR)&serverInfo, sizeof(struct sockaddr));
	if (result != 0)
	{
		printf("bind failed\n");
		return -1;
	}

	// start listening for clients
	result = listen(listener, 10);
	if(result == SOCKET_ERROR)
	{
		printf("listening failed\n");
		return -1;
	}

	printf("Listening on port %hu...\n", port);

	// accept client connection
	while (true)
	{
		SOCKET client = accept(listener, NULL, NULL);
		if (client == INVALID_SOCKET)
		{
			printf("client accept fail\n");
			continue;
		}
		printf("Client connected\n");

		char buffer[MAX_BUFFER_SIZE];
		int bytesReceived = recv(client, buffer, MAX_BUFFER_SIZE - 1, 0);
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

			int bytesSent = send(client, buffer, bytesReceived, 0);
			if (bytesSent == SOCKET_ERROR)
			{
				printf("send failed.\n");
				return -1;
			}
		}

		closesocket(client);
	}


	// Terminate
	closesocket(listener);
	WSACleanup();
	return 0;
}