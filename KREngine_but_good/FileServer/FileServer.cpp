#define _WINSOCK_DEPRICATED_NO_WARNINGS


#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <vector>
#include <experimental/filesystem>
#include <filesystem> 
#include <assert.h>


#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER_SIZE 1024


namespace fs = std::experimental::filesystem;

int main(int argc, char* argv[])
{
	system("color 03");
	uint16_t port = 8888;
	fs::path dataPath = "../Assets/Data/";
	if (argc >= 2)
	{
		port = static_cast<uint16_t>(atoi(argv[1]));
	}
	if (argc >= 3)
	{
		dataPath = argv[2];
	}
	WSAData wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("WSAStartup failed\n");
		return -1;
	}

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //create listener socket
	if (listener == INVALID_SOCKET)
	{
		printf("Failed to create socket\n");
		return -1;
	}
	SOCKADDR_IN serverInfo;
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;
	serverInfo.sin_port = htons(port);

	result = bind(listener, (LPSOCKADDR)&serverInfo, sizeof(struct sockaddr)); //bind socket
	if (result != 0)
	{
		printf("bind failed\n");
		return -1;
	}
	result = listen(listener, 10); //begin listening
	if (result == SOCKET_ERROR)
	{
		printf("listening failed\n");
		return -1;
	}

	while (true) //client communication loop
	{
		SOCKET client = accept(listener, NULL, NULL); //accept client
		if (client == INVALID_SOCKET)
		{
			printf("client accept fail\n");
			continue;
		}
		printf("Client connected\n");
		char buffer[MAX_BUFFER_SIZE];


		assert(fs::is_directory(dataPath)); //check if valid directory
		std::vector<std::string> fileList;
		std::string tempString;
		for (const auto& file : fs::directory_iterator(dataPath)) // read files in directory
		{
			fileList.push_back(fs::path(file).filename().string());
			tempString += fileList.back();
		}
		int bytesSent = send(client, tempString.c_str(), tempString.size(), 0); //send list of files to client
		if (bytesSent == SOCKET_ERROR)
		{
			printf("send failed.\n");
			return -1;
		}

		char receiveBuffer[MAX_BUFFER_SIZE];
		int bytesReceived = recv(client, receiveBuffer, MAX_BUFFER_SIZE - 1, 0); //receive requested file Name
		if (bytesReceived == SOCKET_ERROR)
		{
			printf("recv failed \n");
			return -1;
		}
		if (bytesReceived == 0)
		{
			printf("Connection closed \n");
		}
		receiveBuffer[bytesReceived] = '\0';
		fs::path tempFileName = "../Assets/Data/" + std::string(receiveBuffer);

		if (fs::exists(tempFileName)) //read file bytes
		{
			std::ifstream file(tempFileName, std::ios::binary | std::ios::ate);
			std::ifstream::pos_type pos = file.tellg();
			std::vector<char> fileBuffer(pos);
			file.seekg(0, std::ios::beg);
			file.read(&fileBuffer[0], pos);
			const char* transferFile = &fileBuffer[0];
			std::string fileSizeString = std::to_string(fileBuffer.size());
			bytesSent = send(client, transferFile, fileBuffer.size(), 0); //send block of file data to client
			if (bytesSent == SOCKET_ERROR)
			{
				printf("send failed.\n");
				return -1;
			}
		printf("Sent %s", receiveBuffer);
		}
		else
		{
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
