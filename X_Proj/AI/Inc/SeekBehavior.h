#ifndef INCLUDED_AI_SEEKBEHAVIOR_H
#define INCLUDED_AI_SEEKBEHAVIOR_H

#include "SteeringBehavior.h"

namespace Ai
{

class SeekBehavior : public SteeringBehavior
{
public:
	X::Math::Vector2 Calculate(Agent& agent) override;
	const char* GetName() const override { return "Seek"; }
};

} // namespace AI

#endif // #ifndef INCLUDED_AI_SEEKBEHAVIOR_H