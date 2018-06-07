#pragma once

#include "Component.h"

namespace GameEngine
{

class TransformComponent;

class AABoxColliderComponent : public Component
{
	const TransformComponent* mTransformComponent;
	Math::Vector3 mCenter;
	Math::Vector3 mExtend;
	Math::Vector4 mColor;

	using CollisionEvent = std::function<void()>;
	using CollisionEvents = std::vector<CollisionEvent>;
	CollisionEvents mCollisionEvents;
	CollisionEvents mCollisionEnterEvents;
	CollisionEvents mCollisionExitEvents;

public:
	enum class CollisionEventType
	{
		Enter,
		Exit,
		Colliding,
		COUNT
	};

	REGISTER_TYPE(ABCC); // (A)ABB(B)ox(C)ollider(C)omponent

	static void CreateFunc(GameObject* gameObj, const TiXmlNode* node);

	AABoxColliderComponent();
	~AABoxColliderComponent() override;

	void Initialize() override;
	void Terminate() override;

	void Render() override;

	bool CheckCollision(AABoxColliderComponent& boxB);
	void AddCollisionEvent(CollisionEvent eventCall, CollisionEventType eventType = CollisionEventType::Colliding);
	void OnCollision();
	void OnCollisionEnter();
	void OnCollisionExit();

	void SetCenter(const Math::Vector3& center) { mCenter = center; }
	void SetExtend(const Math::Vector3& extend) { mExtend = extend; }
	void SetColor(const Math::Vector4& color) { mColor = color; }

	Math::AABB GetAABB() const;

};

static bool CheckCollision(AABoxColliderComponent* boxA, AABoxColliderComponent* boxB)
{
	return boxA->CheckCollision(*boxB);
}

} // namespace GameEngine