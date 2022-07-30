/*!
@file EvadeSlimeObject.cpp
@brief EvadeSlimeObjectÇÃÉNÉâÉXé¿ëÃ
íSìñÅFä€éRóTäÏ
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyObjectBase.h"
#include "SlimeObjectBase.h"
#include "EvadeSlimeObject.h"

#include "MetaballRenderBase.h"
#include "GameTimer.h"
#include "TaskList.h"
#include "SeekTargetOffsetsManager.h"
#include "SeekTarget.h"
#include "MetaballChildrenSeekManager.h"
#include "MetaballChildrenPressedManager.h"

#include "PressData.h"
#include "SlimeCore.h"
#include "SlimeRender.h"

#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_EvadeSlime.h"

#include "I_KnockBack.h"
#include "I_PushedWater.h"
#include "SlimeBase.h"
#include "EvadeSlime.h"

#include "AnimationMainStateMachine.h"
#include "AnimatorBase.h"
#include "Animator_EvadeSlime.h"

namespace basecross {
	namespace Enemy {

		namespace EvadeSlime {

			EvadeSlimeObject::EvadeSlimeObject(const std::shared_ptr<Stage>& stage)
				:SlimeObjectBase(stage)
			{}

			void EvadeSlimeObject::OnCreate() {
				AddComponent<EvadeSlime>();
				SlimeObjectBase::OnCreate();

				AddComponent<Stator_EvadeSlime>();
				AddComponent<Animator>();
			}

			void EvadeSlimeObject::CreateModel() {
				//Core
				auto color = Col4(0.5f, 0.1f, 0.1f, 1.0f);
				auto core = AddComponent<SlimeCore>();
				core->SetEyeDiffuse(color);
				//SlimeRender
				Vec3 basePosition(0.0f, -0.0f, 0.0f);
				auto parametor = Metaball::ChildrenSeekManager::Parametor();
				auto offsets = {
					Vec3(0.0f, 0.5f, 0.0f),
					Vec3(0.25f, 0.25f, 0.0f),Vec3(-0.25f, 0.25f, 0.0f),

					Vec3(0.5f, 0.0f, 0.0f),Vec3(0.0f, 0.0f, 0.0f),Vec3(-0.5f, 0.0f, 0.0f),

					Vec3(0.5f, 0.25f, -0.25f),Vec3(0.0f, 0.25f, -0.25f),Vec3(-0.5f, 0.25f, -0.25f),
					Vec3(0.5f, 0.25f, 0.25f),Vec3(0.0f, 0.25f, 0.25f),Vec3(-0.5f, 0.25f, 0.25f),

					Vec3(0.75f, -0.5f, 0.0f),Vec3(0.25f, -0.5f, 0.0f),Vec3(0.0f, -0.5f, 0.0f),Vec3(-0.25f, -0.5f, 0.0f),Vec3(-0.75f, -0.5f, 0.0f),
					Vec3(0.75f, -0.5f, 0.5f),Vec3(0.25f, -0.5f, 0.5f),Vec3(0.0f, -0.5f, +0.5f),Vec3(-0.25f, -0.5f, 0.5f),Vec3(-0.75f, -0.5f, 0.5f),
					Vec3(0.75f, -0.5f, -0.5f),Vec3(0.25f, -0.5f, -0.5f),Vec3(0.0f, -0.5f, -0.5f),Vec3(-0.25f, -0.5f, -0.5f),Vec3(-0.75f, -0.5f, -0.5f),
				};
				for (const auto& offset : offsets) {
					const float Speed = 20.0f;
					const float Range = 0.0f;
					const auto SeekType = SeekTarget::SeekType::Lerp;
					parametor.seekParams.push_back(SeekTarget::Parametor(Speed, Range, basePosition + offset, SeekType));
				}

				const int NumCreate = static_cast<int>(offsets.size());
				auto render = AddComponent<SlimeRender>(NumCreate);
				render->SetChildrenRadius(0.1f);
				AddComponent<Metaball::ChildrenSeekManager>(parametor);
				AddComponent<Metaball::ChildrenPressedManager>();

				render->SetDiffuse(color);
				SetAlphaActive(true);
			}

		}
	}
}