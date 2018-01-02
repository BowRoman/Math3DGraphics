#pragma once


template<typename T>
inline void SafeDelete(T*& ptr)
{
	delete ptr;
	ptr = nullptr;
}

template<typename T>
inline void SafeDeleteArray(T*& ptr)
{
	delete[] ptr;
	ptr = nullptr;
}

template<typename T>
inline void SafeDeleteVector(std::vector<T*>& vec)
{
	for (auto element : vec)
	{
		SafeDelete(element);
	}
	vec.clear();
}

template<typename T>
inline void SafeRelease(T*& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}