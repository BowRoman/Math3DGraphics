#define _WINSOCK_DEPRICATED_NO_WARNINGS

#include <thread>
#include <mutex>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <conio.h>
#include <string>
#include <vector>
#include <deque>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER_SIZE 1024

std::mutex lock;
std::deque<std::string> MessageList;

void ReceiveThread(SOCKET server);
void PrintThread();
void SendThread(std::string name, SOCKET server);

void ReceiveThread(SOCKET server)
{
	//Receive Thread:
	//	- Lock
	//	- Push to back of list
	//	- Unlock
	while (true)
	{
		//printf("waiting on receive thread\n");
		char buffer[MAX_BUFFER_SIZE];
		int bytesReceived = recv(server, buffer, MAX_BUFFER_SIZE, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			printf("recv failed \n");
		}
		if (bytesReceived == 0)
		{
			//return -1
		}
		else
		{
			buffer[bytesReceived] = '\0';
			lock.lock();
			MessageList.push_back(buffer);
			lock.unlock();
		}
	}
}

void PrintThread()
{
	//printf("waiting on print thread\n");
	while (MessageList.size() > 0)
	{
		lock.lock();
		printf("%s\n", MessageList[0].c_str());
		MessageList.pop_front();
		lock.unlock();
	}
}

void SendThread(std::string name, SOCKET server)
{
	while (_kbhit())
	{
		std::string message;
		getline(std::cin, message);
		message = name + ": " + message;
		send(server, message.c_str(), (int)message.length(), 0);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
}


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

	std::string name;
	printf("connected to server\n");
	printf("Enter Name:");
	getline(std::cin, name);

	while (true)
	{
		std::thread receiver(ReceiveThread, severSocket);
		receiver.detach();
		std::thread printer(PrintThread);
		printer.join();
		std::thread sender(SendThread, name, severSocket);
		sender.join();

	}




	// Terminate
	closesocket(severSocket);
	WSACleanup();
	system("pause");
	return 0;
}