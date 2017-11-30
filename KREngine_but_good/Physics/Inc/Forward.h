#pragma once

namespace Physics
{

class Constraint;
class Particle;

typedef std::vector<Particle*>		ParticleVec;
typedef std::vector<Constraint*>	ConstraintVec;
typedef std::vector<Math::OBB>		OBBVec;

} // namespace Physics