#include "Precompiled.h"
#include "NetworkUtil.h"
#include "StreamReader.h"
#include "MemoryStream.h"


Network::StreamReader::StreamReader(MemoryStream& memoryStream)
	:mMemoryStream(memoryStream)
{

}

Network::StreamReader::~StreamReader()
{

}

template<typename T>
inline void Network::StreamReader::Read(T& info)
{
	std::memcpy(info, mMemoryStream.mBuffer + mMemoryStream.mHead, sizeof(info));


}

void Network::StreamReader::Read(std::string& info)
{
	std::memcpy((void*)info[0], mMemoryStream.mBuffer + mMemoryStream.mHead, info.length());
}