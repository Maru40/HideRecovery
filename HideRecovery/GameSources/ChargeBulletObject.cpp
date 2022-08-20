
/*!
@file ChargeBulletObject.cpp
@brief ChargeBulletObject�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "ChargeBulletObject.h"

#include "ChargeBullet.h"

namespace basecross {

	ChargeBulletObject::ChargeBulletObject(const std::shared_ptr<Stage>& stage)
		:BulletObjectBase(stage)
	{}

	void ChargeBulletObject::OnCreate() {
		SettingModel();

		AddComponent<ChargeBullet>();

		auto collision = AddComponent<CollisionObb>();
		collision->SetAfterCollision(AfterCollision::None);
	}

	void ChargeBulletObject::SettingModel() {
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");

		transform->SetScale(Vec3(0.25f));
	}

}