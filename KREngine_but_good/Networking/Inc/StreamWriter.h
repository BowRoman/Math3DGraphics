#pragma once



namespace Network
{
	class MemoryStream;
	class StreamWriter
	{
		MemoryStream& mMemoryStream;
	public:
		StreamWriter(MemoryStream& memoryStream);
		~StreamWriter();

		template<typename T>
		void Write(const T& info);

		void Write(const std::string& info);


	};
}