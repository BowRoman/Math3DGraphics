#include "Agent.h"
#include "PerceptionModule.h"

using namespace Ai;

PerceptionModule::PerceptionModule(Agent& agent)
	: mAgent{ agent }
{
}

PerceptionModule::~PerceptionModule()
{
}

void PerceptionModule::Purge()
{
	for (auto s : mSensors)
	{
		X::SafeDelete(s);
	}
	mSensors.clear();
}

void PerceptionModule::Update(float deltaTime, ImportanceCalculator impCalc)
{
	// Allow agent to percieve environment
	for (auto s : mSensors)
	{
		s->Update(mAgent,mMemoryRecords,deltaTime);
	}

	// Set record importances and remove expired ones
	for (auto iter = mMemoryRecords.begin(); iter != mMemoryRecords.end(); ++iter)
	{
		auto record{ *iter };
		if (record->lastRecordedTime + mAgent.MemorySpan() < X::TimeSeconds())
		{
			X::SafeDelete(record);
			iter = mMemoryRecords.erase(iter);
		}
		else
		{
			impCalc(*record);
		}
	}

	// Sort by importance
	mMemoryRecords.sort([](MemoryRecord* a, MemoryRecord* b)
	{
		return a->importance > b->importance;
	});
}
