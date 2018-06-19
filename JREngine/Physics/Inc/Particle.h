#pragma once
#include <limits>

namespace Physics
{

class Particle
{
	friend class PhysicsWorld;
	friend class Spring;
	friend class Fixed;
	friend class PhysicsOBB;
	friend class PhysicsPlane;
	friend class PlaneConstraint;

	Math::Vector3 mPosition = Math::Vector3::Zero();
	Math::Vector3 mPositionOld = Math::Vector3::Zero();
	Math::Vector3 mAcceleration = Math::Vector3::Zero();

	float mCreationTime = 0.0f;
	float mLifespan = (std::numeric_limits<float>::max)();

	float mRadius = 0.1f;
	float mInvMass = 238.7324206f; // based on volume assuming radius 0.1f
	float mDensity = 1.0f;
	bool bImplicitMass = true; // If false, the mass will not change unless set by the user

public:
	Particle();
	Particle(float lifespan);
	Particle(const Math::Vector3& position);
	Particle(const Math::Vector3& position, float lifeSpan);

	void DebugDraw() const;

	void SetPosition(Math::Vector3 position) { mPosition = position; mPositionOld = position; }
	void SetVelocity(Math::Vector3 velocity) { mPositionOld = mPosition - velocity; }

	void SetRadius(float radius);
	void SetDensity(float density);
	void SetInvMass(float invMass) { bImplicitMass = false; mInvMass = invMass; }
	void SetMass(float mass);

	void UseImplicitMass(bool bImplicit = true) { bImplicitMass = bImplicit; }

private:
	void CalculateImplicitMass();
}; // class Particle

} // namespace Physics