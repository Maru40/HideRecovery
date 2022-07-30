
/*!
@file BigShootSlimeObject.cpp
@brief BigShootSlimeObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyObjectBase.h"
#include "SlimeObjectBase.h"

#include "BigShootSlimeObject.h"

#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_BigShootSlime.h"

#include "AnimatorBase.h"
#include "MaruAnimationHelper.h"
#include "AnimationMainStateMachine.h"
#include "Animator_NormalSlime.h"

#include "I_KnockBack.h"
#include "I_PushedWater.h"
#include "SlimeBase.h"
#include "BigSlimeBase.h"
#include "BigShootSlime.h"

#include "AbsorbedSlimeManager.h"

#include "PressCenterManager.h"

#include "MetaballChildrenPressedManager.h"

#include "EnemyObjectBase.h"
#include "ShootSlimeObject.h"

#include "StatorBase.h"
#include "Stator_ShootSlime.h"

#include "ShootManager.h"

namespace basecross {
	namespace Enemy {
		namespace BigShootSlime {

			BigShootSlimeObject::BigShootSlimeObject(const std::shared_ptr<Stage>& stage)
				:SlimeObjectBase(stage)
			{}

			void BigShootSlimeObject::OnCreate() {
				AddComponent<BigShootSlime>();
				SlimeObjectBase::OnCreate();

				//ステーター
				AddComponent<Stator>();
				//シュート管理
				AddComponent<ShootSlime::ShootManager>();

				//吸収Slime管理のコンポーネント
				auto absorbedMangaer = AddComponent<BigSlime::AbsorbedSlimeManager<ShootSlime::ShootSlimeObject>>();
				absorbedMangaer->ChangeStates<ShootSlime::Stator_ShootSlime>(ShootSlime::Stator_ShootSlime::State::Absorbed);
				//あたり判定の調整
				GetComponent<CollisionObb>()->SetMakedSize((float)absorbedMangaer->GetAbsorbedObjects().size());
			}

			void BigShootSlimeObject::CreateModel() {
				AddComponent<Metaball::ChildrenPressedManager>();
			}

			void BigShootSlimeObject::SettingScalePop() {

			}

		}
	}
}

