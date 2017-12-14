#include "Precompiled.h"
#include "PhysicsPlane.h"

#include "Particle.h"

using namespace Physics;

PhysicsPlane::PhysicsPlane()
	: mPlane{Math::Plane()}
	, mFriction{1.0f}
	, mRestitution{1.0f}
{
}

PhysicsPlane::PhysicsPlane(Math::Plane const& plane, float friction, float restitution)
	: mPlane{ plane }
	, mFriction{ friction }
	, mRestitution{ restitution }
{
}

void Physics::PhysicsPlane::Apply(ParticleVec& particles)
{
	for (auto particle : particles)
	{
		Physics::Particle par = *particle;
		// project position vector onto plane normal
		float distance{ Math::Dot(mPlane.n, par.mPosition) };
		float distanceOld{ Math::Dot(mPlane.n, par.mPositionOld) };

		// if distance is less than plane radius, the point is below the plane.
		if (distance < mPlane.d && distanceOld >= mPlane.d)
		{
			// Calculate velocity
			auto velocity = par.mPosition - par.mPositionOld;

			// Calculate reflection vector
			auto velocityVert = (mPlane.n * (Math::Dot(velocity, mPlane.n)));
			auto reflection = velocity - (velocityVert * 2.0f);
			// Project reflection vector onto normal
			auto reflectionVert = (mPlane.n * (Math::Dot(reflection, mPlane.n)));

			// Move particle position above plane
			particle->mPosition += reflectionVert;

			// Apply friction and restitution variables to reflection
			auto reflectionHor = reflection - reflectionVert;

			if (Math::MagnitudeSqr(reflectionVert) <= 0.0001f) // magic number
			{
				reflectionVert = Math::Vector3::Zero();
			}
			else
			{
				reflectionVert *= mRestitution;
			}
			if (Math::MagnitudeSqr(reflectionHor) <= 0.0001f) // magic number
			{
				reflectionHor = Math::Vector3::Zero();
			}
			else
			{
				reflectionHor *= mFriction;
			}
			reflection = reflectionVert + reflectionHor;

			// Move particle old position
			particle->SetVelocity(reflection);
		}
	}
}