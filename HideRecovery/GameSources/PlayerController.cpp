
/*!
@file PlayerObject.cpp
@brief PlayerObject�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "PlayerController.h"

#include "RotationController.h"
#include "PlayerInputer.h"
#include "MaruUtility.h"

namespace basecross {

	PlayerController::PlayerController(const std::shared_ptr<GameObject>& objPtr):
		Component(objPtr)
	{}

	void PlayerController::OnStart() {
		m_rotationController = GetGameObject()->GetComponent<RotationController>();
	}

	void PlayerController::OnUpdate() {
		Rotation();
	}

	void PlayerController::Rotation() {
		auto input = PlayerInputer::GetMoveDirection();
		auto direct = maru::Utility::CalcuCameraVec(Vec3(input.x, 0, input.y), GetStage()->GetView()->GetTargetCamera(), GetGameObject());

		m_rotationController.lock()->SetDirect(direct);
	}

}