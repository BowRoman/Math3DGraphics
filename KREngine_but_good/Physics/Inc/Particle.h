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
	void SetRadius(float radius) { mRadius = radius; }
	void SetInvMass(float invMass) { mInvMass = invMass; }
	void SetMass(float mass);

private:
	Hello World;
	Hello Spring;
	Math::Vector3 mPosition;
	Math::Vector3 mPositionOld;
	Math::Vector3 mAcceleration;

	float mRadius;
	float mInvMass;
}; // class Particle

} // namespace Physics