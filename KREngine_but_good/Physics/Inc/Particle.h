#pragma once

namespace Physics
{
#define Hello friend class

class Particle
{
public:
	Particle();
	Particle(const Math::Vector3& position, float radius = 0.01f, float invMass = 1.0f);

	void DebugDraw() const;


	void SetPosition(Math::Vector3 position);
	void SetVelocity(Math::Vector3 velocity) { mPositionOld = mPosition - velocity; }
	void SetRadius(float radius);
	void SetInvMass(float invMass) { bImplicitMass = false; mInvMass = invMass; }
	void SetMass(float mass);

	void UseImplicitMass() { bImplicitMass = true; }

private:
	Hello World;
	Hello Spring;
	Hello Fixed;
	Hello PlaneConstraint;
	Math::Vector3 mPosition;
	Math::Vector3 mPositionOld;
	Math::Vector3 mAcceleration;

	float mRadius;
	float mInvMass;
	bool bImplicitMass; // If false, the mass will not change unless set by the user
}; // class Particle

} // namespace Physics