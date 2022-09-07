
/*!
@file BillBoard_Ex.cpp
@brief BillBoard_Ex�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "BillBoard_Ex.h"

namespace basecross {

	BillBoard_Ex::BillBoard_Ex(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr)
	{}

	void BillBoard_Ex::OnUpdate() {
		// �J�������擾����
		auto camera = GetStage()->GetView()->GetTargetCamera();

		// �J�����ւ̉�]���Z�o����
		auto boardToCamera = camera->GetEye() - transform->GetPosition();

		transform->SetForward(boardToCamera);
	}

}