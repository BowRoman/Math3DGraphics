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

	SOCKADDR_IN recAddress;
	int recAddressSize = sizeof(recAddress);
	recAddress.sin_family = AF_INET;
	recAddress.sin_port = htons(port);
	recAddress.sin_addr.s_addr = inet_addr("255.255.255.255");

	bool enabled = true;
	if (setsockopt(Socket, SOL_SOCKET, SO_BROADCAST, (char*)&enabled, sizeof(bool)) < 0)
	{
		printf("broadcast set up falied");
		WSACleanup();
		return -1;
	}
	while (true) //broadcast
	{

		char buffer[MAX_BUFFER_SIZE] = "Is anybody there?";
		int bytesSent = sendto(Socket, buffer, MAX_BUFFER_SIZE, 0, (SOCKADDR*)&recAddress, recAddressSize);
		if (bytesSent == SOCKET_ERROR)
		{
			printf("send failed.\n");
			WSACleanup();
			return -1;
		}
		printf("Broadcasting To Port %d: %s\n", port, buffer);

		Sleep(1000);
	}


	// Terminate
	closesocket(Socket);
	WSACleanup();
	return 0;
}