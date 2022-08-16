#include "stdafx.h"
#include "ObjectMover.h"
#include "VelocityManager.h"

namespace basecross
{
namespace Operator
{
	ObjectMover::ObjectMover(const std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{
	}

	void ObjectMover::OnCreate()
	{
		m_velocityManager = GetGameObject()->GetComponent<VelocityManager>();
	}

	Vec3 ObjectMover::Move(const Vec2& moveDirection)
	{
		auto velocityManager = m_velocityManager.lock();

		if (moveDirection.lengthSqr() == 0)
		{
			if (velocityManager) {
				velocityManager->ResetAll();
			}

			return Vec3();
		}

		Vec3 inputVector;

		inputVector.x = moveDirection.x;
		inputVector.z = moveDirection.y;

		Vec3 forward = Vec3(0, 0, 1);
		Vec3 right = Vec3(1, 0, 0);

		auto camera = m_camera.lock();

		if (camera && m_isCameraAffected)
		{
			Vec3 cameraForward = camera->GetAt() - camera->GetEye();
			cameraForward.y = 0;
			cameraForward.normalize();
			forward = cameraForward;
			right = Vec3(cameraForward.z, 0, -cameraForward.x);
		}

		auto moveForward = forward * inputVector.z;
		auto moveRight = right * inputVector.x;

		Vec3 moveVector = (moveForward + moveRight) * App::GetApp()->GetElapsedTime() * m_moveSpeed;

		if (velocityManager)
		{
			velocityManager->SetVelocity(moveVector / App::GetApp()->GetElapsedTime());
		}
		else
		{
			auto position = transform->GetPosition();

			position += moveVector;
			transform->SetPosition(position);
		}

		return moveVector;
	}
}
}