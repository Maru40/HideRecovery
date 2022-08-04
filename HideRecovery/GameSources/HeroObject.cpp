/*!
@file EyeSearchRangeParam.cpp
@brief EyeSearchRangeParam‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "HeroObject.h"

#include "Hero.h"

#include "EnemyMainStateMachine.h"
#include "Stator_Hero.h"

#include "RotationController.h"
#include "VelocityManager.h"

namespace basecross {
	namespace Enemy {

		HeroObject::HeroObject(const std::shared_ptr<Stage>& stage):
			EnemyObjectBase(stage)
		{}

		void HeroObject::OnCreate() {
			EnemyObjectBase::OnCreate();

			SettingModel();

			AddComponent<Hero>();
			AddComponent<Stator_Hero>();

			//‹¤’Ê---------------------------------------

			AddComponent<CollisionObb>();
			AddComponent<RotationController>();
			AddComponent<Gravity>();
			AddComponent<VelocityManager>();

			//-------------------------------------------
		}

		void HeroObject::SettingModel() {
			auto draw = AddComponent<PNTStaticDraw>();
			draw->SetMeshResource(L"DEFAULT_CUBE");

			draw->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 1.0f));

			transform->SetScale(Vec3(1.0f, 1.5f, 1.0f));
		}
	}
}