#include "Precompiled.h"
#include "PhysicsPlane.h"

using namespace Physics;

PhysicsPlane::PhysicsPlane()
	: mPlane{Math::Plane()}
	, mFriction{1.0f}
	, mRestitution{1.0f}
{
}

PhysicsPlane::PhysicsPlane(Math::Plane const& plane, float friction, float restitution)
	: mPlane{ plane }
	, mFriction{ 1.0f }
	, mRestitution{ 1.0f }
{
}