#include"PlayerCameraMover.h"
#include"PlayerInputer.h"

namespace basecross
{
	PlayerCameraMover::PlayerCameraMover(std::shared_ptr<GameObject>& owner) :
		Component(owner),
		m_minRotX(-XM_PIDIV2),
		m_maxRotX(XM_PIDIV2)
	{

	}

	void PlayerCameraMover::SetMinRotX(const float minRotX)
	{
		m_minRotX = minRotX;
	}

	float PlayerCameraMover::GetMinRotX() const
	{
		return m_minRotX;
	}

	void PlayerCameraMover::SetMaxRotX(const float maxRotX)
	{
		m_maxRotX = maxRotX;
	}

	float PlayerCameraMover::GetMaxRotX() const
	{
		return m_maxRotX;
	}

	void PlayerCameraMover::SetForward(const Vec3& forward)
	{
		transform->SetForward(forward);

		auto rotation = transform->GetQuaternion().toRotVec();

		m_cameraRot = rotation;

		m_cameraRot.x = MyMath::Clamp(m_cameraRot.x, m_minRotX, m_maxRotX);

		transform->SetRotation(m_cameraRot.x, m_cameraRot.y, 0);
	}

	void PlayerCameraMover::OnUpdate()
	{
		return;
		auto rotation = PlayerInputer::GetCameraRotation();
		if (rotation == Vec2(0.0f)) {
			return;
		}

		m_cameraRot.y += rotation.x;

		m_cameraRot.x += rotation.y;

		m_cameraRot.x = MyMath::Clamp(m_cameraRot.x, m_minRotX, m_maxRotX);

		transform->SetRotation(m_cameraRot.x, m_cameraRot.y, 0);
	}
}