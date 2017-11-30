#include "Precompiled.h"
#include "World.h"

#include "Constraints.h"
#include "Particle.h"

using namespace Physics;

World::World()
	: mTimer{ 0.0f }
{}

World::~World(){}

void World::Update(float deltaTime)
{
	mTimer += deltaTime;
	while (mTimer >= mSettings.timeStep)
	{
		mTimer -= mSettings.timeStep;
		AccumulateForces();
		Integrate();
		SatisfyConstraints();
	}
}

void World::AddParticle(Particle* p)
{
	mParticles.push_back(p);
}

void World::AddConstraint(Constraint* c)
{
	mConstraints.push_back(c);
}

void World::ClearDynamic()
{
	SafeDeleteVector(mParticles);
	SafeDeleteVector(mConstraints);
}

void World::DebugDraw() const
{
	for (const auto p : mParticles)
	{
		p->DebugDraw();
	}
	for (const auto c : mConstraints)
	{
		c->DebugDraw();
	}
}

void World::AccumulateForces()
{
	for (auto p : mParticles)
	{
		p->mAcceleration = mSettings.gravity;
	}
}

void World::Integrate()
{
	const float timeStepSqr = Math::Sqr(mSettings.timeStep);
	for (auto p : mParticles)
	{
		Math::Vector3 displacement{ p->mPosition - p->mPositionOld + (p->mAcceleration * timeStepSqr) };
		p->mPositionOld = p->mPosition;
		p->mPosition = p->mPosition + displacement;
	}
}

void World::SatisfyConstraints()
{
	for (auto c : mConstraints)
	{
		c->Apply();
	}
}