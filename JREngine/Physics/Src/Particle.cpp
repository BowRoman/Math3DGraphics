#include "Precompiled.h"
#include "Particle.h"

using namespace Physics;

Particle::Particle()
{
}

Physics::Particle::Particle(float lifespan)
	: mLifespan{ lifespan }
{
}

Particle::Particle(const Math::Vector3& position)
	: mPosition{ position }
	, mPositionOld{ position }
{
}

Particle::Particle(const Math::Vector3& position, float lifespan)
	: mPosition{ position }
	, mPositionOld{ position }
	, mLifespan{ lifespan }
{
}

void Particle::DebugDraw() const
{
	Graphics::SimpleDraw::DrawSphere(mPosition, 3, 2, mRadius, Math::Vector4::Cyan());
}

void Particle::SetRadius(float radius)
{
	ASSERT(radius > 0.0f, "[Particle] Radius must be positive and above zero.");
	mRadius = radius;
	CalculateImplicitMass();
}

void Physics::Particle::SetDensity(float density)
{
	ASSERT(density > 0.0f, "[Particle] Density must be positive and above zero.");
	mDensity = density;
	CalculateImplicitMass();
}

// LAZY, SetInvMass() is better
void Particle::SetMass(float mass)
{
	ASSERT(mass > 0.0f, "[Particle] Mass must be positive and above zero.");
	bImplicitMass = false;
	mInvMass = 1 / mass;
}

void Particle::CalculateImplicitMass()
{
	if (bImplicitMass)
	{
		if (mRadius != 0.0f && mDensity != 0.0f)
		{
			// generate implicit mass based on volume of the sphere
			mInvMass = 1 / (mDensity * (1.33333333f * Math::kPi * (mRadius*mRadius*mRadius)));
		}
	}
}
