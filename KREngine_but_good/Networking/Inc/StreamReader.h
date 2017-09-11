#pragma once

namespace Network
{
	class MemoryStream;
	class StreamReader
	{
		MemoryStream& mMemoryStream;
	public:
		StreamReader(MemoryStream& memoryStream);
		~StreamReader();

		template<typename T>
		void Read(T& info);

		void Read(std::string& info);
	};

}
