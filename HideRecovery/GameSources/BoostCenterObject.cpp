/*!
@file BoostCenterObject.cpp
@brief BoostCenterObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "BoostCenterObject.h"

#include "LaserBase.h"
#include "LaserCollider.h"

#include "BuilderVertexPCT.h"
#include "BoardObject.h"
#include "BillBoard.h"

namespace basecross {

	BoostCenterObject::BoostCenterObject(const std::shared_ptr<Stage>& stage)
		:GameObject(stage)
	{}

	void BoostCenterObject::OnCreate() {
		auto param = LaserBaseDraw::Parametor();
		param.longerSpeed = 10.0f;
		param.length = 0.5f;
		param.maxLength = 3.0f;
		param.longerType = ScaleChangeType::Reverse;
		param.texture = L"Boost_TX";
		param.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		auto draw = AddComponent<LaserBaseDraw>(param);

		transform->SetScale(1.0f, 3.0f, 1.0f);
	}

}