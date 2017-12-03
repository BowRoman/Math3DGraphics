#pragma once

#include "MemoryRecord.h"
#include "Sensor.h"

namespace Ai
{
typedef std::function<void(MemoryRecord&)> ImportanceCalculator;

class PerceptionModule
{
public:
	PerceptionModule(Agent& agent);
	~PerceptionModule();

	template<class SensorType>
	SensorType* AddSensor();
	void Purge();

	void Update(float deltaTime, ImportanceCalculator impCalc);
	
	const MemoryRecords& GetMemoryRecords() const { return mMemoryRecords; }

private:
	Agent& mAgent;
	Sensors mSensors;
	MemoryRecords mMemoryRecords;

};

template<class SensorType>
SensorType* PerceptionModule::AddSensor()
{
	static_assert(std::is_base_of<Sensor, SensorType>::value, "[PerceptionModule] Can only add Sensor types.");
	auto sensor = new SensorType();
	mSensors.push_back(sensor);
	return sensor;
}


} // namespace Ai