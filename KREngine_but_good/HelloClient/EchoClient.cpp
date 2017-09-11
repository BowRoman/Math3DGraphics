#define _WINSOCK_DEPRICATED_NO_WARNINGS


#include <iostream>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

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

	// Init
	WSAData wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("WSAStartup failed\n");
		return -1;
	}

	SOCKET severSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (severSocket == INVALID_SOCKET)
	{
		printf("Failed to create socket\n");
		return -1;
	}

	in_addr iaHost;
	iaHost.s_addr = inet_addr(hostAddress);
	HOSTENT* hostEntry = gethostbyaddr((const char*)&iaHost, sizeof(struct in_addr), AF_INET);
	if (hostEntry == nullptr)
	{
		printf("gethostaddr failed\n");
		return -1;
	}

	SOCKADDR_IN serverInfo;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);
	serverInfo.sin_port = htons(port);

	result = connect(severSocket, (LPSOCKADDR)&serverInfo, sizeof(struct sockaddr));
	if (result == SOCKET_ERROR)
	{
		printf("connect failed\n");
		return -1;
	}

	printf("connected to server\n");
	printf("Enter Message:");

	char listBuffer[MAX_BUFFER_SIZE];
	int bytesReceived = recv(severSocket, listBuffer, MAX_BUFFER_SIZE - 1, 0);
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
		listBuffer[bytesReceived] = '\0';
		printf("Received: %s\n", listBuffer);
	}

	std::string message;
	getline(std::cin, message);
	send(severSocket, message.c_str(), (int)message.length(), 0); // request file from server
	
	char receivedDataBuffer[MAX_BUFFER_SIZE];
	std::ofstream outFile(message, std::ios::binary | std::ios::out);
	while(bytesReceived != 0)
	{
		bytesReceived = recv(severSocket, receivedDataBuffer, MAX_BUFFER_SIZE, 0);
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
		else
		{
			outFile.write(receivedDataBuffer, bytesReceived);
		}
	}
	outFile.close();
	


	// Terminate
	closesocket(severSocket);
	WSACleanup();
	system("pause");
	return 0;
}