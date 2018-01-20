#include "Precompiled.h"
#include "Particle.h"

using namespace Physics;

Particle::Particle()
	: mPosition(0.0f, 0.0f, 0.0f)
	, mPositionOld(0.0f, 0.0f, 0.0f)
	, mAcceleration(0.0f, 0.0f, 0.0f)
	, mRadius{ 0.5f }
	, mInvMass{ 0.2387f } // based on volume assuming radius 1.0f
	, bImplicitMass{true}
{
}

Particle::Particle(const Math::Vector3& position, float radius, float invMass)
	: mPosition(position)
	, mPositionOld(position)
	, mAcceleration(0.0f, 0.0f, 0.0f)
	, mRadius{ radius }
	, mInvMass{ invMass }
	, bImplicitMass{ false }
{
}

void Particle::DebugDraw() const
{
	Graphics::SimpleDraw::DrawSphere(mPosition, 3, 2, mRadius, Math::Vector4::Cyan());
}

void Particle::SetPosition(Math::Vector3 position)
{
	mPosition = position;
	mPositionOld = position;
}

void Particle::SetRadius(float radius)
{
	mRadius = radius;
	if (bImplicitMass)
	{
		if (radius != 0.0f)
		{
			// generate implicit mass based on volume of the sphere
			mInvMass = 1 / (1.3333333f * Math::kPi * (radius*radius*radius));
		}
	}
}

// LAZY, SetInvMass() is better
void Particle::SetMass(float mass)
{
	ASSERT(mass > 0.0f, "[Particle] Mass cannot be zero.");
	bImplicitMass = false;
	mInvMass = 1 / mass;
}
