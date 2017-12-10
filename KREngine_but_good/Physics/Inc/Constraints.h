#pragma once

namespace Physics
{

class Constraint
{
public:
	virtual ~Constraint() {}

	virtual void Apply() const = 0;
	virtual void DebugDraw() const {}

}; // class Constraint

class Spring : public Constraint
{
public:
	Spring(Particle* a, Particle* b, float restingDistance);

	void Apply() const override;
	void DebugDraw() const override;

protected:
	Particle* mParticleA;
	Particle* mParticleB;
	float mRestDistance;
};

class Fixed : public Constraint
{
public:
	Fixed(Particle* p, Math::Vector3 position);

	void Apply() const override;
	void DebugDraw() const override;

protected:
	Particle* mParticle;
	Math::Vector3 mPosition;
};

class PlaneConstraint : public Constraint
{
public:
	PlaneConstraint(Particle* p, Math::Plane plane, float invRestitution = 1.0f, float invFriction = 1.0f);

	void Apply() const override;

protected:
	Particle* mParticle;
	Math::Plane mPlane;
	float mRestitution;
	float mFriction;
};

} // namespace Physics