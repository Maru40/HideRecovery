/*!
@file ChargeBulletObject.cpp
@brief ChargeBulletObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ChargeBulletObject.h"

#include "Maruyama/Bullet/Component/ChargeBullet.h"

#include "Watanabe/Effekseer/EfkEffect.h"
#include "Watanabe/Component/LifeSpan.h"

#include "Itabashi/OnlineStatus.h"

namespace basecross {
	ChargeBulletObject::ChargeBulletObject(const std::shared_ptr<Stage>& stage)
		:BulletObjectBase(stage)
	{}

	void ChargeBulletObject::SetTeamType(team::TeamType teamType) {
		switch (teamType)
		{
		case team::TeamType::Blue:
			m_teamTypeStr = L"Blue";
			break;
		case team::TeamType::Red:
			m_teamTypeStr = L"Red";
			break;
		}
		SettingEffect();
	}

	void ChargeBulletObject::OnCreate()
	{
		AddComponent<Online::OnlineStatus>();
		AddComponent<ChargeBullet>();

		auto collision = AddComponent<CollisionObb>();
		collision->SetAfterCollision(AfterCollision::None);

		auto lifeSpan = AddComponent<LifeSpan>();
		lifeSpan->SetLifeTime(1.0f);
	}

	void ChargeBulletObject::SettingEffect() {
		auto efkComp = AddComponent<EfkComponent>();
		efkComp->SetEffectResource(m_teamTypeStr + L"Bullet", TransformData(Vec3(0, 0, 0), Vec3(0.3f)));
		efkComp->IsSyncGameObject(m_teamTypeStr + L"Bullet", true);
		efkComp->SetEffectResource(m_teamTypeStr + L"Hit", TransformData(Vec3(0, 0, 0), Vec3(0.2f)));
		efkComp->Play(m_teamTypeStr + L"Bullet", true);

		transform->SetScale(Vec3(0.25f));
	}

	void ChargeBulletObject::OnDestroy() {
		auto efkComp = GetComponent<EfkComponent>();
		efkComp->Stop(m_teamTypeStr + L"Bullet");
		efkComp->Play(m_teamTypeStr + L"Hit", true);
	}
}