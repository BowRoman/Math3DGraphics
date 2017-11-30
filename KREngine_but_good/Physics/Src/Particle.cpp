#include "Precompiled.h"
#include "Particle.h"

using namespace Physics;

Particle::Particle()
	: mPosition(0.0f, 0.0f, 0.0f)
	, mPositionOld(0.0f, 0.0f, 0.0f)
	, mAcceleration(0.0f, 0.0f, 0.0f)
	, mRadius{ 1.0f }
	, mInvMass{ 1.0f }
{
}

Particle::Particle(const Math::Vector3& position, float radius, float invMass)
	: mPosition(position)
	, mPositionOld(position)
	, mAcceleration(0.0f, 0.0f, 0.0f)
	, mRadius{ radius }
	, mInvMass{ invMass }
{
}

void Particle::DebugDraw() const
{
	Graphics::SimpleDraw::DrawSphere(mPosition, 3, 2, mRadius, Math::Vector4::Strobe());
}

void Particle::SetPosition(Math::Vector3 position)
{
	mPosition = position;
	mPositionOld = position;
}

void Particle::SetMass(float mass)
{
	ASSERT(mass > 0.0f, "[Particle] Mass cannot be zero.");
	mInvMass = 1 / mass;
}
