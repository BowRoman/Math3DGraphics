#include "Precompiled.h"
#include "NetworkUtil.h"
#include "StreamWriter.h"
#include "MemoryStream.h"

Network::StreamWriter::StreamWriter(MemoryStream& memoryStream)
	:mMemoryStream(memoryStream)
{
}


Network::StreamWriter::~StreamWriter()
{
}


template<typename T>
inline void Network::StreamWriter::Write(const T& info)
{
	if (mMemoryStream.mCapacity > mMemoryStream.mHead + sizeof(info))
	{
		std::memcpy(mMemoryStream.mBuffer + mMemoryStream.mHead, info, sizeof(info));
	}
	else
	{
		mMemoryStream.ReallocBuffer(mMemoryStream.mCapacity + sizeof(info) * 2);
		std::memcpy(mMemoryStream.mBuffer + mMemoryStream.mHead, info, sizeof(info));
	}

	mMemoryStream.mHead += sizeof(info);

}

void Network::StreamWriter::Write(const std::string& info)
{
	if (mMemoryStream.mCapacity > mMemoryStream.mHead + info.length())
	{
		std::memcpy(mMemoryStream.mBuffer + mMemoryStream.mHead, info.c_str(), info.length());
	}
	else
	{
		mMemoryStream.ReallocBuffer(mMemoryStream.mCapacity + info.length() * 2);
		std::memcpy(mMemoryStream.mBuffer + mMemoryStream.mHead, info.c_str(), info.length());
	}

	mMemoryStream.mHead += info.length();
}