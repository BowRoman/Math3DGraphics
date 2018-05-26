#include "Precompiled.h"
#include "World.h"

#include "Constraints.h"
#include "Particle.h"
#include "PhysicsOBB.h"
#include "PhysicsPlane.h"

using namespace Physics;

PhysicsWorld::PhysicsWorld()
{}

PhysicsWorld::~PhysicsWorld()
{
	ASSERT(mParticles.empty(), "[PhysicsWorld] Particle vector must be cleared before destruction.");
	ASSERT(mConstraints.empty(), "[PhysicsWorld] Constraint vector must be cleared before destruction.");
	ASSERT(mPlanes.empty(), "[PhysicsWorld] Plane vector must be cleared before destruction.");
}

void PhysicsWorld::Update(float deltaTime)
{
	mTimer += deltaTime;
	mWorldTime += deltaTime;
	while (mTimer >= mSettings.timeStep)
	{
		mTimer -= mSettings.timeStep;
		AccumulateForces();
		Integrate();
		SatisfyConstraints();
		RemoveExpired(); // TODO: Add support for constraint removal
	}
}

void PhysicsWorld::AddParticle(Particle* p)
{
	p->mCreationTime = mWorldTime;
	mParticles.push_back(p);
}

void PhysicsWorld::AddConstraint(Constraint* c)
{
	mConstraints.push_back(c);
}

void Physics::PhysicsWorld::AddPhysicsPlane(PhysicsPlane* p)
{
	mPlanes.push_back(p);
}

void Physics::PhysicsWorld::AddPhysicsOBB(PhysicsOBB* obb)
{
	mOBBs.push_back(obb);
}

void Physics::PhysicsWorld::AddCube(Physics::PhysicsWorld& world, Math::Vector3 position, Math::Vector3 velocity, float width, float invMass, bool fixed)
{
	float particleinvMass = invMass / 8;
	float halfWidth = width * 0.5f;

	Particle* particles[8];
	for (int i = 0; i < 8; ++i)
	{
		auto p = new Physics::Particle();
		p->SetRadius(0.3f);
		p->SetInvMass(particleinvMass);
		particles[i] = p;
		world.AddParticle(p);
	}
	// top left
	particles[0]->SetPosition({ position.x - halfWidth, position.y + halfWidth, position.z + halfWidth });
	// top right
	particles[1]->SetPosition({ position.x + halfWidth, position.y + halfWidth, position.z + halfWidth });
	// bottom right
	particles[2]->SetPosition({ position.x + halfWidth, position.y + halfWidth, position.z - halfWidth });
	// bottom left
	particles[3]->SetPosition({ position.x - halfWidth, position.y + halfWidth, position.z - halfWidth });

	// top left
	particles[4]->SetPosition({ position.x - halfWidth, position.y - halfWidth, position.z + halfWidth });
	// top right
	particles[5]->SetPosition({ position.x + halfWidth, position.y - halfWidth, position.z + halfWidth });
	// bottom right
	particles[6]->SetPosition({ position.x + halfWidth, position.y - halfWidth, position.z - halfWidth });
	// bottom left
	particles[7]->SetPosition({ position.x - halfWidth, position.y - halfWidth, position.z - halfWidth });

	for (int i = 0; i < 8; ++i)
	{
		particles[i]->SetVelocity(velocity);
	}

	// top square
	for (int i = 0; i < 4; ++i)
	{
		auto c0 = new Physics::Spring(particles[i], particles[(i + 1) % 4], width);
		auto c1 = new Physics::Spring(particles[i], particles[(i + 4)], width);

		world.AddConstraint(c0);
		world.AddConstraint(c1);
	}
	// bottom square
	for (int i = 0; i < 4; ++i)
	{
		auto c = new Physics::Spring(particles[i + 4], particles[((i + 1) % 4) + 4], width);
		world.AddConstraint(c);
	}

	float squareDiagonalLength = Math::Sqrt(Math::Sqr(width) * 2.0f); // pythagoras
	//float cubeDiagonalLength = Math::Sqrt(Math::Sqr(width) * Math::Sqr(squareDiagonalLength));

	// diagonals across top
	for (int i = 0; i < 4; ++i)
	{
		auto c = new Physics::Spring(particles[i], particles[(i + 2) % 4], squareDiagonalLength);
		world.AddConstraint(c);
	}

	// diagonals across bottom
	for (int i = 0; i < 4; ++i)
	{
		auto c = new Physics::Spring(particles[i + 4], particles[((i + 2) % 4) + 4], squareDiagonalLength);
		world.AddConstraint(c);
	}

	// diagonals across sides
	// backslash
	auto c = new Physics::Spring(particles[0], particles[5], squareDiagonalLength);
	world.AddConstraint(c);

	c = new Physics::Spring(particles[1], particles[6], squareDiagonalLength);
	world.AddConstraint(c);

	c = new Physics::Spring(particles[2], particles[7], squareDiagonalLength);
	world.AddConstraint(c);

	c = new Physics::Spring(particles[3], particles[4], squareDiagonalLength);
	world.AddConstraint(c);

	// forward slash
	c = new Physics::Spring(particles[0], particles[7], squareDiagonalLength);
	world.AddConstraint(c);

	c = new Physics::Spring(particles[1], particles[4], squareDiagonalLength);
	world.AddConstraint(c);

	c = new Physics::Spring(particles[2], particles[5], squareDiagonalLength);
	world.AddConstraint(c);

	c = new Physics::Spring(particles[3], particles[6], squareDiagonalLength);
	world.AddConstraint(c);

	// diagonals across center
	//c = new Physics::Spring(particles[0], particles[6], cubeDiagonalLength);
	//world.AddConstraint(c);
	//
	//c = new Physics::Spring(particles[1], particles[7], cubeDiagonalLength);
	//world.AddConstraint(c);
	//
	//c = new Physics::Spring(particles[2], particles[4], cubeDiagonalLength);
	//world.AddConstraint(c);
	//
	//c = new Physics::Spring(particles[3], particles[5], cubeDiagonalLength);
	//world.AddConstraint(c);

	if (fixed)
	{
		auto f = new Physics::Fixed(particles[0], particles[0]->mPosition);
		world.AddConstraint(f);
		f = new Physics::Fixed(particles[1], particles[1]->mPosition);
		world.AddConstraint(f);
		f = new Physics::Fixed(particles[2], particles[2]->mPosition);
		world.AddConstraint(f);
		f = new Physics::Fixed(particles[3], particles[3]->mPosition);
		world.AddConstraint(f);
		f = new Physics::Fixed(particles[4], particles[4]->mPosition);
		world.AddConstraint(f);
		f = new Physics::Fixed(particles[5], particles[5]->mPosition);
		world.AddConstraint(f);
		f = new Physics::Fixed(particles[6], particles[6]->mPosition);
		world.AddConstraint(f);
		f = new Physics::Fixed(particles[7], particles[7]->mPosition);
		world.AddConstraint(f);
	}

}

// Clears all pointer containers
void PhysicsWorld::ClearDynamic()
{
	SafeDeleteVector(mParticles);
	SafeDeleteVector(mConstraints);
	SafeDeleteVector(mPlanes);
	SafeDeleteVector(mOBBs);
}

// Clears particle and constraint containers
void PhysicsWorld::ClearParticles()
{
	SafeDeleteVector(mParticles);
	SafeDeleteVector(mConstraints);
}

void PhysicsWorld::DebugDraw() const
{
	for (const auto p : mParticles)
	{
		p->DebugDraw();
	}
	for (const auto c : mConstraints)
	{
		c->DebugDraw();
	}
	for (const auto o : mOBBs)
	{
		o->DebugDraw();
	}
}

void PhysicsWorld::AccumulateForces()
{
	for (auto p : mParticles)
	{
		p->mAcceleration = mSettings.gravity;
	}
}

void PhysicsWorld::Integrate()
{
	const float timeStepSqr = Math::Sqr(mSettings.timeStep);
	for (auto p : mParticles)
	{
		Math::Vector3 displacement{ p->mPosition - p->mPositionOld + (p->mAcceleration * timeStepSqr) };
		p->mPositionOld = p->mPosition;
		p->mPosition = p->mPosition + displacement;
	}
}

void PhysicsWorld::SatisfyConstraints()
{
	for (auto c : mConstraints)
	{
		c->Apply();
	}

	for (auto p : mPlanes)
	{
		p->Apply(mParticles);
	}

	for (auto o : mOBBs)
	{
		o->Apply(mParticles);
	}
}

void PhysicsWorld::RemoveExpired()
{
	auto i = mParticles.begin();
	while(i != mParticles.end())
	{
		auto p = *i;
		if (mWorldTime > p->mCreationTime + p->mLifespan)
		{
			i = mParticles.erase(i);
		}
		else
		{
			++i;
		}
	}
}
