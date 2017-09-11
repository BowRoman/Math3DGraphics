#pragma once

namespace Graphics
{
	class Transform
	{
	public:
		Transform();
		Transform(const Math::Vector3& position, const Math::Vector3& direction);
		~Transform();


		void Walk(float distance);
		void Strafe(float distance);
		void Rise(float distance);


		void Yaw(float rad);
		void Pitch(float rad);

		Math::Matrix4 GetWorldMatrix() const;

		void SetPosition(const Math::Vector3& position);
		void SetDirection(const Math::Vector3& direction);


		inline const Math::Vector3& GetPosition() const { return mPosition; }
		inline const Math::Vector3& GetDirection() const { return mDirection; }

	private:
		Math::Vector3 mPosition;
		Math::Vector3 mDirection;
	};
}
