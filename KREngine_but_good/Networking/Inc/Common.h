#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")


//#include <assert.h>
#include <functional>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <string>
#include <unordered_map>

#include <Core\Inc\Core.h>


