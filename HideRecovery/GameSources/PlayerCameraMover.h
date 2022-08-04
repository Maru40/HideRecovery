#pragma once

namespace basecross
{
	class PlayerCameraMover : public Component
	{
		Vec2 m_cameraRot;

		float m_minRotX;
		float m_maxRotX;

	public:
		PlayerCameraMover(std::shared_ptr<GameObject>& owner);

		void SetMinRotX(const float minRotX);

		float GetMinRotX() const;

		void SetMaxRotX(const float maxRotX);

		float GetMaxRotX() const;

		void SetForward(const Vec3& forward);

		void OnUpdate() override;
	};
}