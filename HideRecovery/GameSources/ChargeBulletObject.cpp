
/*!
@file ChargeBulletObject.cpp
@brief ChargeBulletObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ChargeBulletObject.h"

#include "ChargeBullet.h"

namespace basecross {

	ChargeBulletObject::ChargeBulletObject(const std::shared_ptr<Stage>& stage)
		:GameObject(stage)
	{}

	void ChargeBulletObject::OnCreate() {
		SettingModel();

		AddComponent<ChargeBullet>();
	}

	void ChargeBulletObject::SettingModel() {
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");

		transform->SetScale(Vec3(0.25f));
	}

}