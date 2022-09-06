
/*!
@file BillBoard_Ex.cpp
@brief BillBoard_Exクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "BillBoard_Ex.h"

namespace basecross {

	BillBoard_Ex::BillBoard_Ex(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr)
	{}

	void BillBoard_Ex::OnUpdate() {
		// カメラを取得する
		auto camera = GetStage()->GetView()->GetTargetCamera();

		// カメラへの回転を算出する
		auto boardToCamera = camera->GetEye() - transform->GetPosition();

		transform->SetForward(boardToCamera);
	}

}