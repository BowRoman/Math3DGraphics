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
	system("color 02");
	uint16_t port = 7777;
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
		WSACleanup();
		return -1;
	}

	SOCKET Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (Socket == INVALID_SOCKET)
	{
		printf("Failed to create socket\n");
		WSACleanup();
		return -1;
	}


	SOCKADDR_IN ServerAddress;
	int serverAddressSize = sizeof(ServerAddress);

	SOCKADDR_IN sendAddress;
	int sendAddressSize = sizeof(sendAddress);
	sendAddress.sin_family = AF_INET;
	sendAddress.sin_port = htons(port);
	sendAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	result = bind(Socket, (SOCKADDR*)&sendAddress, sizeof(sendAddress));
	if (result < 0)
	{
		printf("bind failed\n");
		WSACleanup();
		return -1;
	}
	printf("bind complete\n");

	while (true) //broadcast to send socket
	{
		char receiveBuffer[MAX_BUFFER_SIZE];
		int bytesReceived = recvfrom(Socket, receiveBuffer, MAX_BUFFER_SIZE, 0, (SOCKADDR*)&ServerAddress, &serverAddressSize);
		printf("Received Message From Port %d: %s\n", port, receiveBuffer);
		Sleep(1000);
	}

	// Terminate
	closesocket(Socket);
	WSACleanup();
	return 0;
}