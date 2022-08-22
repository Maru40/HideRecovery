﻿#include "stdafx.h"
#include "ObjectMover.h"
#include "VelocityManager.h"

#include "MaruUtility.h"

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

	Vec3 ObjectMover::NormalMove(const Vec2& moveDirection) {
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

		auto camera = m_camera.lock();

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

	Vec3 ObjectMover::AimMove(const Vec2& moveDirection) {
		Vec3 resultVec = NormalMove(moveDirection);

		return resultVec;
	}

	Vec3 ObjectMover::Move(const Vec2& moveDirection)
	{
		if (IsAim()) {
			return AimMove(moveDirection);
		}
		else {
			return NormalMove(moveDirection);
		}
	}
}
}