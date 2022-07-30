/*!
@file WaterAuraObject.cpp
@brief WaterAuraObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "WaterAuraObject.h"

#include "WaterCollisionManager.h"
#include "LaserBase.h"
#include "LaserCollider.h"

namespace basecross {

	WaterAuraObject::WaterAuraObject(const std::shared_ptr<Stage>& stage)
		:GameObject(stage)
	{}

	void WaterAuraObject::OnCreate() {
		AddComponent<WaterCollisionManager>();

		auto param = LaserBaseDraw::Parametor();
		param.width = 0.5f;
		param.color = Col4(1.0f, 1.0f, 1.0f, 0.0f);
		auto draw = AddComponent<LaserBaseDraw>(param);
		draw->SetLongerType(ScaleChangeType::Reverse);

		AddTag(L"Water");
	}

}