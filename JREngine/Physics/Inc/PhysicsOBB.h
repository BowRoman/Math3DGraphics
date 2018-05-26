#pragma once

namespace Physics
{

class PhysicsOBB
{
public:
	PhysicsOBB();
	PhysicsOBB(Math::OBB const& obb, float restitution = 1.0f);
	~PhysicsOBB() {}

	void Apply(ParticleVec& particles);
	void DebugDraw();

private:
	void SetMaxExtend();

private:
	friend class PhysicsWorld;

	Math::OBB mOBB;
	float mMaxExtend;
	float mRestitution;

}; // class PhysicsPlane

} // namespace Physics