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

Wall::Wall(Particle* p, Math::Plane plane)
	: mParticle{ p }
	, mPlane{ plane }
{
}

void Wall::Apply() const
{
	auto pos = mParticle->mPosition;
	// dot product
	float distance{ (mPlane.n.x * pos.x) + (mPlane.n.y * pos.y) + (mPlane.n.z * pos.z) + mPlane.d };
	// if distance is less than plane radius, the point is below the plane.
	if (distance < mPlane.d)
	{
		//auto reflect = ;
		mParticle->mPosition = mParticle->mPositionOld;
		//mParticle->mPositionOld = reflect;
	}
}
