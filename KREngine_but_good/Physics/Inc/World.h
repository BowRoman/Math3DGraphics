#pragma once

namespace Physics
{

struct Settings
{
	Settings()
		: gravity(0.0f,-9.81f,0.0f)
		, timeStep{1.0f/60.0f}
		, drag{0.0f}
		, iterations{1}
	{}

	Math::Vector3 gravity;
	float timeStep;
	float drag;
	uint32_t iterations;
};

class World
{
public:
	World();
	~World();

	void Setup(const Settings& settings) { mSettings = settings; }

	void Update(float deltaTime);

	void AddParticle(Particle* p);
	void AddConstraint(Constraint* c);
	void ClearDynamic();

	void DebugDraw() const;

private:
	void AccumulateForces();
	void Integrate();
	void SatisfyConstraints();

	Settings mSettings;
	ParticleVec mParticles;
	ConstraintVec mConstraints;
	float mTimer;

}; // class World

} // namespace Physics