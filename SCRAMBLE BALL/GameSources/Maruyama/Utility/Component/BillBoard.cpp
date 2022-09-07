
/*!
@file BillBoard.cpp
@brief BillBoard�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"
#include "BillBoard.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Utility/Mathf.h"

namespace basecross {

	void BillBoard::OnUpdate() {
		// �J�������擾����
		auto camera = dynamic_pointer_cast<Camera>(GetStage()->GetView()->GetTargetCamera());

		auto transComp = GetGameObject()->GetComponent<Transform>();
		auto pos = transComp->GetPosition();

		// �J�����ւ̉�]���Z�o����
		auto boardToCamera = camera->GetEye() - pos;
		//boardToCamera.y = 0.0f;
		boardToCamera.normalize();
		float rotY = atan2f(-boardToCamera.z, boardToCamera.x) + maru::Utility::FOWARD_ADJUST;

		auto rotation = transComp->GetRotation();
		boardToCamera.y = 0.0f;
		transComp->SetForward(boardToCamera);

		//��]����offset
		Quat quat = transComp->GetQuaternion();
		quat *= (Quat)XMQuaternionRotationAxis(transComp->GetRight(), m_rotationOffset.x);
		quat *= (Quat)XMQuaternionRotationAxis(transComp->GetUp(), m_rotationOffset.y);
		quat *= (Quat)XMQuaternionRotationAxis(transComp->GetForward(), m_rotationOffset.z);

		transComp->SetQuaternion(quat);

	}

}

//endbasecross