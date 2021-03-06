#pragma once

#include "Component.h"

namespace GameEngine
{

class TransformComponent : public Component
{
	Math::Vector3 mPosition;
	Math::Vector3 mForward;

public:
	REGISTER_TYPE(TFMC); // (T)rans(f)or(m)(C)omponent

	static void CreateFunc(GameObject* gameObj, const TiXmlNode* node);

	TransformComponent();
	~TransformComponent() override;

	Math::Matrix4 GetLocalToWorld() const;

	void SetPosition(const Math::Vector3& pos) { mPosition = pos; }
	void SetForward(const Math::Vector3& forward) { mForward = forward; }

	const Math::Vector3& GetPosition() const { return mPosition; }
	const Math::Vector3& GetForward() const { return mForward; }

};

} // namespace GameEngine