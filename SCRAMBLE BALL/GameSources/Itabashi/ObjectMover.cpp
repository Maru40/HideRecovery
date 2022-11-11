#include "stdafx.h"
#include "ObjectMover.h"
#include "VelocityManager.h"

namespace basecross
{
namespace Operator
{
	ObjectMover::ObjectMover(const std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{}

	void ObjectMover::OnLateStart()
	{
		m_velocityManager = GetGameObject()->GetComponent<VelocityManager>();
	}

	Vec3 ObjectMover::Move(const Vec2& moveDirection) {
		auto velocityManager = m_velocityManager.lock();

		// 移動量が0ならば
		if (moveDirection.lengthSqr() == 0)
		{
			velocityManager->ResetAll();
			return Vec3();
		}

		Vec3 inputVector;

		inputVector.x = moveDirection.x;
		inputVector.z = moveDirection.y;

		auto camera = m_camera.lock();

		// カメラがあり影響を受けるならば
		if (camera && m_isCameraAffected)
		{
			Vec3 cameraForward = camera->GetAt() - camera->GetEye();
			cameraForward.y = 0;
			cameraForward.normalize();
			m_defaultForward = cameraForward;
		}

		Vec3 right = Vec3(m_defaultForward.z, 0, -m_defaultForward.x);


		auto moveForward = m_defaultForward * inputVector.z;
		auto moveRight = right * inputVector.x;
		auto moveSpeed = m_moveSpeed;

		Vec3 moveVector = (moveForward + moveRight) * moveSpeed;

		velocityManager->SetVelocity(moveVector);

		return moveVector;
	}
}
}