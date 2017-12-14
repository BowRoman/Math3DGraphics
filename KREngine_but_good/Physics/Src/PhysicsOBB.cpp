#include "Precompiled.h"
#include "PhysicsOBB.h"

#include "Particle.h"

using namespace Physics;

PhysicsOBB::PhysicsOBB()
	: mOBB{ Math::OBB() }
	, mRestitution{ 1.0f }
{
	SetMaxExtend();
}

PhysicsOBB::PhysicsOBB(Math::OBB const& obb, float restitution)
	: mOBB{ obb }
	, mRestitution{ restitution }
{
	SetMaxExtend();
}

void Physics::PhysicsOBB::Apply(ParticleVec& particles)
{
	for (auto particle : particles)
	{
		if (Math::Distance(particle->mPosition, mOBB.center) <= mMaxExtend)
		{

		}
	}
}

void PhysicsOBB::DebugDraw()
{
	auto xExtend = Math::Vector3(mOBB.extend.x, 0.0f, 0.0f);
	auto yExtend = Math::Vector3(0.0f, mOBB.extend.y, 0.0f);
	auto zExtend = Math::Vector3(0.0f, 0.0f, mOBB.extend.z);
	auto top = mOBB.center + yExtend;
	auto top1 = top + zExtend + xExtend;
	auto top2 = top + zExtend - xExtend;
	auto top3 = top - zExtend - xExtend;
	auto top4 = top - zExtend + xExtend;
	auto bot = mOBB.center - yExtend;
	auto bot1 = bot + zExtend + xExtend;
	auto bot2 = bot + zExtend - xExtend;
	auto bot3 = bot - zExtend - xExtend;
	auto bot4 = bot - zExtend + xExtend;

	Graphics::SimpleDraw::DrawLine(top1, top2, Math::Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(top2, top3, Math::Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(top3, top4, Math::Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(top4, top1, Math::Vector4::Orange());

	Graphics::SimpleDraw::DrawLine(top1, bot1, Math::Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(top2, bot2, Math::Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(top3, bot3, Math::Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(top4, bot4, Math::Vector4::Orange());

	Graphics::SimpleDraw::DrawLine(bot1, bot2, Math::Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(bot2, bot3, Math::Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(bot3, bot4, Math::Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(bot4, bot1, Math::Vector4::Orange());
}

void Physics::PhysicsOBB::SetMaxExtend()
{
	auto extend = mOBB.extend;
	float longest = extend.x;
	float secondLongest = extend.y;
	if (extend.y > longest)
	{
		secondLongest = longest;
		longest = extend.y;
	}
	if (extend.z > longest)
	{
		secondLongest = longest;
		longest = extend.z;
	}
	float squareDiagonalLength = Math::Sqr(longest) * Math::Sqr(secondLongest);
	mMaxExtend = Math::Sqrt(squareDiagonalLength * Math::Sqr(secondLongest));
}
