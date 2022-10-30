
/*!
@file PlayerController.cpp
@brief PlayerController�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "PlayerController.h"

#include "Maruyama/Utility/Component/RotationController.h"
#include "Patch/PlayerInputer.h"
#include "Maruyama/Utility/Utility.h"

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

		m_rotationController.lock()->SetDirection(direct);
	}

}