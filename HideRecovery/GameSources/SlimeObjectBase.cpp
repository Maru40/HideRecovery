/*!
@file SlimeObjectBase.cpp
@brief SlimeObjectBaseÇÃÉNÉâÉXé¿ëÃ
íSìñÅFä€éRóTäÏ
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyObjectBase.h"
#include "SlimeObjectBase.h"

#include "I_PushedWater.h"
#include "SlimeBase.h"
#include "MetaballRenderBase.h"
#include "SlimeRender.h"
#include "FoundManager.h"
#include "PressCenterManager.h"

#include "BcPNTSlimeDraw.h"
#include "MetaballChildrenRender.h"

#include "VelocityManager.h"
#include "PressRefrectionManager.h"
#include "RotationController.h"

#include "PlayerInputer.h"

#include "TargetManager.h"
#include "SeekTarget.h"

#include "CollisionAction.h"
#include "EyeSearchRange.h"
#include "SlimeCore.h"

#include "EnemyStatusBase.h"
#include "SlimeStatus.h"

#include "GameTimer.h"
#include "TaskList.h"
#include "SeekTargetOffsetsManager.h"
#include "MetaballChildrenSeekManager.h"
#include "MetaballChildrenPressedManager.h"

#include "MetaballMotionManager.h"

#include "SeekTarget.h"

#include "ScalePop.h"

#include "ToTargetMove.h"
#include "Targeted.h"

#include "TactileCtrl.h"
#include "TactileObject.h"
#include "WallEvasion.h"
#include "Freeze.h"

#include "SoleImpactMap.h"

#include "DebugObject.h"

namespace basecross {
	namespace Enemy {

		SlimeObjectBase::SlimeObjectBase(const std::shared_ptr<Stage>& stage)
			:EnemyObjectBase(stage)
		{}

		void SlimeObjectBase::OnCreate()
		{
			EnemyObjectBase::OnCreate();

			CreateModel();

			AddComponent<Gravity>();
			auto col = AddComponent<CollisionObb>();
			//col->SetDrawActive(true);
			const float ColSize = 1.5f;
			col->SetMakedSize(ColSize);

			AddComponent<FoundManager>();
			AddComponent<PressCenterManager>();

			auto velocityManager = AddComponent<VelocityManager>();
			constexpr float MaxVelocitySpeed = 30.0f;
			velocityManager->SetMaxSpeed(MaxVelocitySpeed);
			AddComponent<RotationController>();
			AddComponent<PressRefrectionManager>();

			AddComponent<TargetManager>();

			AddComponent<EyeSearchRange>();
			AddComponent<maru::CollisionAction>();
			AddComponent<ToTargetMove>();
			AddComponent<Targeted>();

			SettingSeek();
			SettingScalePop();
			//SettingWallEvasion();
			SettingSoleImpactMap();

			AddComponent<SlimeStatusManager>();
		}

		void SlimeObjectBase::CreateModel() {}

		void SlimeObjectBase::SettingSeek() {
			SeekTarget::Parametor param;
			param.offset = Vec3(0.0f);
			param.range = 0.0f;
			param.seekType = SeekTarget::SeekType::VelcoityArrive;
			param.speed = 1.5f;

			AddComponent<SeekTarget>(nullptr, param);
		}

		void SlimeObjectBase::SettingScalePop() {
			return;
			auto param = ScalePop::Parametor();
			param.maxScale = Vec3(1.5f);
			param.returnScale = Vec3(1.5f);
			param.speed = 1.0f;

			AddComponent<ScalePop>(param);
		}

		void SlimeObjectBase::SettingWallEvasion() {
			struct Pair {
				Vec3 offset;
				Vec3 rotation;

				Pair(const Vec3& offset, const Vec3& rotation) :
					offset(offset), rotation(rotation)
				{}
			};

			auto pos = transform->GetPosition();
			Pair pairs[] = {
				{ Pair(Vec3(+0.75f, 0.15f, 0.5f), Vec3(0.0f, +XM_PIDIV4, 0.0f)) },
				{ Pair(Vec3(-0.75f, 0.15f, 0.5f), Vec3(0.0f, -XM_PIDIV4, 0.0f)) },
			};

			for (const auto& pair : pairs) {
				auto tactileObject = GetStage()->Instantiate<TactileObject>();
				auto trans = tactileObject->GetComponent<Transform>();
				trans->SetPosition(pos + pair.offset);
				trans->SetRotation(pair.rotation);
				tactileObject->SetParent(GetThis<GameObject>());

				auto tactileComp = tactileObject->GetComponent<TactileCtrl>(false);
				if (tactileComp) {
					AddComponent<WallEvasion>()->SetTactile(tactileComp);
				}
			}
		}

		void SlimeObjectBase::SettingSoleImpactMap() {
			//åªç›í≤êÆíiäKÇÃÇΩÇﬂreturn
			return;
			auto param = maru::SoleImpactMap::Parametor();
			param.factoryParam.rect.width = 15.0f;
			param.factoryParam.rect.depth = 15.0f;
			param.factoryParam.intervalRange = 3.0f;
			AddComponent<maru::SoleImpactMap>(param);
		}

	}
}

//endbasecross