#ifndef INCLUDED_AI_SEPARATIONBEHAVIOR_H
#define INCLUDED_AI_SEPARATIONBEHAVIOR_H

#include "SteeringBehavior.h"

namespace Ai
{

	class SeparationBehavior : public SteeringBehavior
	{
	public:
		X::Math::Vector2 Calculate(Agent& agent) override;
		const char* GetName() const override { return "Separate"; }
	};

} // namespace AI

#endif // #ifndef INCLUDED_AI_SEPARATIONBEHAVIOR_H