#pragma once

#include <XEngine.h>

namespace Ai
{
class Agent;

class MemoryRecord
{
public:
	enum class Status { Confirmed, Suspected };
	MemoryRecord()
		: agent{ nullptr }
		, lastKnownPosition{ 0.0f,0.0f }
		, lastRecordedTime{ 0.0f }
		, importance{ 0.0f }
		, status{ Status::Suspected }
	{}

	Agent* agent;
	X::Math::Vector2 lastKnownPosition;
	float lastRecordedTime;
	float importance;
	Status status;
}; // class MemoryRecord
typedef std::list<MemoryRecord*> MemoryRecords;

} // namespace Ai