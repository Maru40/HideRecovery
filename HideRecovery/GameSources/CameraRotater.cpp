#include"CameraRotater.h"
#include"PlayerInputer.h"
#include"MathHelper.h"

using itbs::Math::MyMath;

namespace basecross
{
	CameraRotater::CameraRotater(std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{

	}

	void CameraRotater::SetMaxRotX(const float maxRotX)
	{
		m_maxRotX = maxRotX;
	}

	float CameraRotater::GetMaxRotX() const
	{
		return m_maxRotX;
	}

	void CameraRotater::SetMinRotX(const float minRotX)
	{
		m_minRotX = minRotX;
	}

	float CameraRotater::GetMinRotX() const
	{
		return m_minRotX;
	}

	void CameraRotater::OnUpdate()
	{
		return;
		auto rotation = transform->GetQuaternion().toRotVec();

		auto rotationVector = PlayerInputer::GetCameraRotation();

		m_rotX -= rotationVector.y;

		m_rotX = MyMath::Clamp(m_rotX, m_minRotX, m_maxRotX);

		transform->SetRotation(m_rotX, 0, 0);
	}
}