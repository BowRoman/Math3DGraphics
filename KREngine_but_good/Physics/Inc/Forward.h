#pragma once

namespace Physics
{

class Constraint;
class Particle;
class PhysicsPlane;

typedef std::vector<Particle*>		ParticleVec;
typedef std::vector<Constraint*>	ConstraintVec;
typedef std::vector<Math::OBB>		OBBVec;
typedef std::vector<PhysicsPlane*>	PhysicsPlaneVec;

} // namespace Physics