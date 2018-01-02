#pragma once

namespace Physics
{

class Constraint;
class Particle;
class PhysicsPlane;
class PhysicsOBB;

typedef std::vector<Particle*>		ParticleVec;
typedef std::vector<Constraint*>	ConstraintVec;
typedef std::vector<Math::OBB>		OBBVec;
typedef std::vector<PhysicsPlane*>	PhysicsPlaneVec;
typedef std::vector<PhysicsOBB*>	PhysicsOBBVec;

} // namespace Physics