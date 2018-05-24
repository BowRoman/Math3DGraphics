#pragma once

#include "Common.h"

template<class DataType>
class HandlePool;

template<class DataType>
class Handle
{
	friend class  HandlePool<DataType>;
	static HandlePool<DataType>* sPool;
	uint32_t mIndex : 24;
	uint32_t mGeneration : 8;

public:
	Handle();

	bool IsValid() const;
	void InValidate();

	DataType* Get() const;

	bool operator==(Handle rhs) const { return mIndex == rhs.mIndex && mGeneration == rhs.mGeneration; }
	bool operator!=(Handle rhs) const { return (*this != rhs); }
};