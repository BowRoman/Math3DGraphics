#define _WINSOCK_DEPRICATED_NO_WARNINGS

#include <thread>
#include <mutex>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#include <deque>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER_SIZE 1024

const int kMax = 10;
std::mutex lock;
std::vector<SOCKET> ClientList;
std::deque<std::string> MessageList;

void AcceptThread(SOCKET listener);
void ReceiveThread(SOCKET client);
void SendThread();

void AcceptThread(SOCKET listener)
{
	//Accept Thread:
	//	- Lock
	//	- Push to back of list
	//	- Unlock
	std::thread receiver[kMax];
	while (true)
	{
		//printf("waiting on accept thread\n");
		SOCKET client = accept(listener, NULL, NULL);
		if (client == INVALID_SOCKET)
		{
			printf("client accept fail\n");
			continue;
		}

		lock.lock();
		ClientList.push_back(client);
		for (uint32_t i = 0; i < kMax; ++i)
		{
			if (i < ClientList.size())
			{
				receiver[i] = std::thread(ReceiveThread, ClientList[i]);
				receiver[i].detach();
			}
		}
		lock.unlock();

		printf("Client connected\n");
	}
}

void ReceiveThread(SOCKET client)
{
	//Receive Thread:
	//	- Lock
	//	- Push to back of list
	//	- Unlock
	while (true)
	{
		//printf("waiting on receive thread\n");
		char buffer[MAX_BUFFER_SIZE];
		int bytesReceived = recv(client, buffer, MAX_BUFFER_SIZE-1, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			printf("recv failed \n");
			break;
		}
		if (bytesReceived == 0)
		{
			break;
		}
		else
		{
			buffer[bytesReceived] = '\0';
			lock.lock();
			printf("Received: %s\n", buffer);
			MessageList.push_back(buffer);
			lock.unlock();
		}
	}
}

void SendThread()
{
	//Send Thread:
	//	- Lock
	//	- cout front of Message List to server
	//	- Send front of Message List to all Clients in Clients List
	//	- pop front of Message List
	//	- Unlock
	while (true)
	{
		//printf("waiting on send thread\n");
		if (MessageList.size() > 0)
		{
			lock.lock();
			printf("Sending: %s\n", MessageList[0].c_str());
			for (uint32_t i = 0; i < ClientList.size(); ++i)
			{
				int bytesSent = send(ClientList[i], MessageList[0].c_str(), MessageList[0].length(), 0);
				if (bytesSent == SOCKET_ERROR)
				{
					printf("send failed.\n");
				}
			}
			MessageList.pop_front();
			lock.unlock();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

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
		WSACleanup();
		return -1;
	}
	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listener == INVALID_SOCKET)
	{
		printf("Failed to create socket\n");
		WSACleanup();
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
		WSACleanup();
		return -1;
	}

	// start listening for clients
	result = listen(listener, kMax);
	if (result == SOCKET_ERROR)
	{
		printf("listening failed\n");
		return -1;
	}
	printf("Listening on port %hu...\n", port);


	while (true)
	{
		std::thread accepter(AcceptThread, listener);
		std::thread sender(SendThread);

		accepter.join();
		sender.join();
	}
	for (uint32_t i = 0; i < ClientList.size(); ++i)
	{
		closesocket(ClientList[i]);
	}

	// Terminate
	closesocket(listener);
	WSACleanup();
	return 0;
}