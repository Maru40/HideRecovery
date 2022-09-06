
/*!
@file BillBoard.cpp
@brief BillBoardƒNƒ‰ƒXÀ‘Ì
’S“–FŠÛR—TŠì
*/

#include "stdafx.h"
#include "Project.h"
#include "BillBoard.h"

#include "Maruyama/Utility/Utility.h"
#include "Mathf.h"

namespace basecross {

	void BillBoard::OnUpdate() {
		// ƒJƒƒ‰‚ğæ“¾‚·‚é
		auto camera = dynamic_pointer_cast<Camera>(GetStage()->GetView()->GetTargetCamera());

		auto transComp = GetGameObject()->GetComponent<Transform>();
		auto pos = transComp->GetPosition();

		// ƒJƒƒ‰‚Ö‚Ì‰ñ“]‚ğZo‚·‚é
		auto boardToCamera = camera->GetEye() - pos;
		//boardToCamera.y = 0.0f;
		boardToCamera.normalize();
		float rotY = atan2f(-boardToCamera.z, boardToCamera.x) + maru::Utility::FOWARD_ADJUST;

		auto rotation = transComp->GetRotation();
		boardToCamera.y = 0.0f;
		transComp->SetForward(boardToCamera);

		//‰ñ“]’²®offset
		Quat quat = transComp->GetQuaternion();
		quat *= (Quat)XMQuaternionRotationAxis(transComp->GetRight(), m_rotationOffset.x);
		quat *= (Quat)XMQuaternionRotationAxis(transComp->GetUp(), m_rotationOffset.y);
		quat *= (Quat)XMQuaternionRotationAxis(transComp->GetForward(), m_rotationOffset.z);

		transComp->SetQuaternion(quat);

	}

}

//endbasecross