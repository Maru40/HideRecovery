/*!
@file ChargeBulletObject.cpp
@brief ChargeBulletObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ChargeBulletObject.h"

#include "ChargeBullet.h"

#include "Watanabe/Effekseer/EfkEffect.h"

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
		//auto draw = AddComponent<PNTStaticDraw>();
		//draw->SetMeshResource(L"DEFAULT_CUBE");

		auto efkComp = AddComponent<EfkComponent>();
		efkComp->SetEffectResource(L"Bullet", TransformData(Vec3(0, 0, 0), Vec3(0.3f)));
		efkComp->IsSyncGameObject(L"Bullet", true);
		efkComp->Play(L"Bullet", true);

		transform->SetScale(Vec3(0.25f));
	}

	void ChargeBulletObject::OnDestroy() {
		GetComponent<EfkComponent>()->Stop(L"Bullet");
	}
}