#pragma once

namespace Physics
{

class PhysicsPlane
{
public:
	PhysicsPlane();
	PhysicsPlane(Math::Plane const& plane, float friction = 1.0f, float restitution = 1.0f);
	~PhysicsPlane() {}

	void Apply(ParticleVec& particles);
	void Apply(Particle* particles);
private:
	friend class World;

	Math::Plane mPlane;
	float mRestitution;
	float mFriction;

}; // class PhysicsPlane

} // namespace Physics