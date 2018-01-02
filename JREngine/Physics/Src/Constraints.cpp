#include "Precompiled.h"
#include "Constraints.h"

#include "Particle.h"

using namespace Physics;

Spring::Spring(Particle* a, Particle* b, float restingDistance)
	: mParticleA(a)
	, mParticleB(b)
	, mRestDistance(restingDistance)
{
}

void Spring::Apply() const
{
	//ASSERT((mParticleA->mPosition != mParticleB->mPosition), "[Constraints] Cannot properly apply spring to particles at the same location. (divide by zero)");
	Math::Vector3 delta = mParticleB->mPosition - mParticleA->mPosition;
	float distance{ Math::Magnitude(delta) };
	float diff{ (distance - mRestDistance) / (distance*(mParticleA->mInvMass + mParticleB->mInvMass)) };
	mParticleA->mPosition += delta * diff * mParticleA->mInvMass;
	mParticleB->mPosition -= delta * diff * mParticleB->mInvMass;
}

void Spring::DebugDraw() const
{
	Graphics::SimpleDraw::DrawLine(mParticleA->mPosition, mParticleB->mPosition, Math::Vector4::Green());
}

Fixed::Fixed(Particle* p, Math::Vector3 position)
	: mParticle(p)
	, mPosition(position)
{
}

void Fixed::Apply() const
{
	mParticle->SetPosition(mPosition);
}

void Fixed::DebugDraw() const
{
	Graphics::SimpleDraw::DrawSphere(mParticle->mPosition, 4, 4, mParticle->mRadius*1.3f, Math::Vector4::Red());
}

PlaneConstraint::PlaneConstraint(Particle* p, Math::Plane plane, float restitution, float friction)
	: mParticle{ p }
	, mPlane{ plane }
	, mRestitution{ restitution }
	, mFriction{ friction }
{
}

void PlaneConstraint::Apply() const
{
	// project position vector onto plane normal
	float distance{ Math::Dot(mPlane.n, mParticle->mPosition) };
	float distanceOld{ Math::Dot(mPlane.n, mParticle->mPositionOld) };

	// if distance is less than plane radius, the point is below the plane.
	if (distance < mPlane.d && distanceOld >= mPlane.d)
	{
		// Calculate velocity
		auto velocity = mParticle->mPosition - mParticle->mPositionOld;

		// Calculate reflection vector
		auto reflection = velocity - (mPlane.n * 2.0f * (Math::Dot(velocity, mPlane.n)));
		// Project reflection vector onto normal
		auto reflectionVert = (mPlane.n * (Math::Dot(reflection, mPlane.n)));

		// Move particle position above plane
		mParticle->mPosition += reflectionVert;

		// Apply friction and restitution variables to reflection
		auto reflectionHor = reflection - reflectionVert;
		reflectionHor *= mFriction;
		reflectionVert *= mRestitution;
		reflection = reflectionVert + reflectionHor;

		// Move particle old position
		mParticle->SetVelocity(reflection);
	}
}
