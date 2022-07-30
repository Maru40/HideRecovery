/*!
@file WaterBodyObject.cpp
@brief WaterBodyObject�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "WaterBodyObject.h"

#include "WaterCollisionManager.h"
#include "LaserBase.h"
#include "LaserCollider.h"

namespace basecross {

	WaterBodyObject::WaterBodyObject(const std::shared_ptr<Stage>& stage)
		:GameObject(stage)
	{}

	void WaterBodyObject::OnCreate() {
		AddComponent<WaterCollisionManager>();

		auto param = LaserBaseDraw::Parametor();
		param.width = 0.15f;
		param.color = Col4(1.0f, 1.0f, 1.0f, 0.0f);
		param.maxLength =15;
		auto draw = AddComponent<LaserBaseDraw>(param);
		draw->SetLongerType(ScaleChangeType::Reverse);
		AddComponent<LaserCollider>();

		AddTag(L"Water");
	}

}