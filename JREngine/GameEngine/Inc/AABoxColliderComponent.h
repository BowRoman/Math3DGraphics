#pragma once

#include "Component.h"

class TransformComponent;

class AABoxColliderComponent : public Component
{
	const TransformComponent* mTransformComponent;
	Math::Vector3 mCenter;
	Math::Vector3 mExtend;
	Math::Vector4 mColor;

public:
	REGISTER_TYPE(ABCC); // (A)ABB(B)ox(C)ollider(Component)

	const static void CreateFunc(GameObject* gameObj, const TiXmlNode* node);

	AABoxColliderComponent();
	~AABoxColliderComponent() override;

	void Initialize() override;
	void Render() override;

	bool CheckCollision(AABoxColliderComponent& boxB);

	void SetCenter(const Math::Vector3& center) { mCenter = center; }
	void SetExtend(const Math::Vector3& extend) { mExtend = extend; }
	void SetColor(const Math::Vector4& color) { mColor = color; }

	Math::AABB GetAABB() const;

};

static bool CheckCollision(AABoxColliderComponent& boxA, AABoxColliderComponent& boxB)
{
	return boxA.CheckCollision(boxB);
}