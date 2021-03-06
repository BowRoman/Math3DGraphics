#include "Precompiled.h"
#include "PhysicsOBB.h"

#include "Particle.h"
#include "PhysicsPlane.h"

using namespace Math;
using namespace Physics;

PhysicsOBB::PhysicsOBB()
	: mOBB{ OBB() }
	, mRestitution{ 1.0f }
{
	SetMaxExtend();
}

PhysicsOBB::PhysicsOBB(OBB const& obb, float restitution)
	: mOBB{ obb }
	, mRestitution{ restitution }
{
	SetMaxExtend();
}

void Physics::PhysicsOBB::Apply(ParticleVec& particles)
{
	for (auto particle : particles)
	{
		if (Math::Intersect(particle->mPosition, mOBB))
		{
			const Matrix4 trans = Math::GetTransform(mOBB);

			particle->mPosition = Math::TransformCoord(particle->mPosition, Math::Inverse(trans));
			particle->mPositionOld = Math::TransformCoord(particle->mPositionOld, Math::Inverse(trans));

			Vector3 oldPos = particle->mPositionOld;
			oldPos.x = Math::Clamp(oldPos.x, mOBB.extend.x * -1.0f, mOBB.extend.x);
			oldPos.y = Math::Clamp(oldPos.y, mOBB.extend.y * -1.0f, mOBB.extend.y);
			oldPos.z = Math::Clamp(oldPos.z, mOBB.extend.z * -1.0f, mOBB.extend.z);

			enum class ClosestAxis
			{
				XPos,
				XNeg,
				YPos,
				YNeg,
				ZPos,
				ZNeg
			};

			ClosestAxis closestAxis = ClosestAxis::XPos;
			float closestDist;

			float posXDist = Math::DistanceSqr(oldPos.x, (mOBB.extend.x));
			float negXDist = Math::DistanceSqr(oldPos.x, (mOBB.extend.x * -1.0f));
			float posYDist = Math::DistanceSqr(oldPos.y, (mOBB.extend.y));
			float negYDist = Math::DistanceSqr(oldPos.y, (mOBB.extend.y * -1.0f));
			float posZDist = Math::DistanceSqr(oldPos.z, (mOBB.extend.z));
			float negZDist = Math::DistanceSqr(oldPos.z, (mOBB.extend.z * -1.0f));

			closestDist = posXDist;
			if (closestDist > negXDist)
			{
				closestAxis = ClosestAxis::XNeg;
				closestDist = negXDist;
			}
			if (closestDist > posYDist)
			{
				closestAxis = ClosestAxis::YPos;
				closestDist = posYDist;
			}
			if (closestDist > negYDist)
			{
				closestAxis = ClosestAxis::YNeg;
				closestDist = negYDist;
			}
			if (closestDist > posZDist)
			{
				closestAxis = ClosestAxis::ZPos;
				closestDist = posZDist;
			}
			if (closestDist > negZDist)
			{
				closestAxis = ClosestAxis::ZNeg;
				closestDist = negZDist;
			}

			// EVALUATE: Possibly better to store the planes instead of generating on collision
			switch (closestAxis)
			{
			case ClosestAxis::XPos:
			{
				Physics::PhysicsPlane plane(Plane{ Math::Vector3::XAxis(), (mOBB.extend.x) }, mRestitution, mRestitution);
				plane.Apply(particle);
				break;
			}
			case ClosestAxis::XNeg:
			{
				Physics::PhysicsPlane plane(Plane{ Vector3::Zero() - Math::Vector3::XAxis(), (mOBB.extend.x) }, mRestitution, mRestitution);
				plane.Apply(particle);
				break;
			}
			case ClosestAxis::YPos:
			{
				Physics::PhysicsPlane plane(Plane{ Math::Vector3::YAxis(), mOBB.extend.y*1.0f }, mRestitution, mRestitution);
				plane.Apply(particle);
				break;
			}
			case ClosestAxis::YNeg:
			{
				Physics::PhysicsPlane plane(Plane{ Vector3::Zero() - Math::Vector3::YAxis(), mOBB.extend.y }, mRestitution, mRestitution);
				plane.Apply(particle);
				break;
			}
			case ClosestAxis::ZPos:
			{
				Physics::PhysicsPlane plane(Plane{ Math::Vector3::ZAxis(), (mOBB.extend.z) }, mRestitution, mRestitution);
				plane.Apply(particle);
				break;
			}
			case ClosestAxis::ZNeg:
			{
				Physics::PhysicsPlane plane(Plane{ Vector3::Zero() - Math::Vector3::ZAxis(), (mOBB.extend.z) }, mRestitution, mRestitution);
				plane.Apply(particle);
				break;
			}
			}

			particle->mPosition = Math::TransformCoord(particle->mPosition, trans);
			particle->mPositionOld = Math::TransformCoord(particle->mPositionOld, trans);
		}
	}
}

void PhysicsOBB::DebugDraw()
{
	auto rotMat = Math::Matrix4::RotationQuaternion(mOBB.rot);
	auto xExtend = Vector3(mOBB.extend.x, 0.0f, 0.0f);
	xExtend = Math::TransformCoord(xExtend, rotMat);
	auto yExtend = Vector3(0.0f, mOBB.extend.y, 0.0f);
	yExtend = Math::TransformCoord(yExtend, rotMat);
	auto zExtend = Vector3(0.0f, 0.0f, mOBB.extend.z);
	zExtend = Math::TransformCoord(zExtend, rotMat);

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

	Graphics::SimpleDraw::DrawLine(top1, top2, Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(top2, top3, Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(top3, top4, Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(top4, top1, Vector4::Orange());

	Graphics::SimpleDraw::DrawLine(top1, bot1, Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(top2, bot2, Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(top3, bot3, Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(top4, bot4, Vector4::Orange());

	Graphics::SimpleDraw::DrawLine(bot1, bot2, Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(bot2, bot3, Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(bot3, bot4, Vector4::Orange());
	Graphics::SimpleDraw::DrawLine(bot4, bot1, Vector4::Orange());
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
	float squareDiagonalLength = Sqr(longest) * Sqr(secondLongest);
	mMaxExtend = Sqrt(squareDiagonalLength * Sqr(secondLongest));
}
