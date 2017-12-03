#pragma once

namespace Physics
{
#define Hello friend class

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

class Wall : public Constraint
{
public:
	Wall(Particle* p, Math::Plane plane);

	void Apply() const override;

protected:
	Particle* mParticle;
	Math::Plane mPlane;
};

} // namespace Physics