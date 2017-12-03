#pragma once

#include "MemoryRecord.h"

namespace Ai
{
class Agent;

class Sensor
{
public:
	Sensor() {}
	virtual ~Sensor() {}

	virtual void Update(Agent& agent, MemoryRecords& memory, float deltaTime) = 0;
	virtual const char* GetName() const = 0;
}; // class Sensor
typedef std::vector<Sensor*> Sensors;

} // namespace Ai