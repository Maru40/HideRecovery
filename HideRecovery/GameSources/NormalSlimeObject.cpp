/*!
@file NormalSlimeObject.cpp
@brief NormalSlimeObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyObjectBase.h"
#include "SlimeObjectBase.h"

#include "NormalSlimeObject.h"

#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_NormalSlime.h"

#include "AnimatorBase.h"
#include "MaruAnimationHelper.h"
#include "AnimationMainStateMachine.h"
#include "Animator_NormalSlime.h"

#include "I_KnockBack.h"
#include "I_PushedWater.h"
#include "SlimeBase.h"
#include "NormalSlime.h"

#include "CollisionAction.h"
#include "EyeSearchRange.h"
#include "SlimeCore.h"

#include "SeekTarget.h"
#include "GameTimer.h"
#include "TaskList.h"
#include "SeekTargetOffsetsManager.h"
#include "MetaballChildrenSeekManager.h"
#include "MetaballChildrenPressedManager.h"

#include "MetaballMotionManager.h"

#include "MetaballRenderBase.h"
#include "SlimeRender.h"

#include "MetaballChildrenRender.h"

namespace basecross {
	namespace Enemy {

		NormalSlimeObject::NormalSlimeObject(const std::shared_ptr<Stage>& stage)
			:SlimeObjectBase(stage)
		{}

		void NormalSlimeObject::OnCreate() {
			AddComponent<NormalSlime::NormalSlime>();
			SlimeObjectBase::OnCreate();

			AddComponent<Stator_NormalSlime>();
			AddComponent<NormalSlime::Animator>();
		}

		void NormalSlimeObject::CreateModel() {
			AddComponent<SlimeCore>();
			//SlimeRender
			Vec3 base(0.0f, -0.0f, 0.0f);
			auto parametor = Metaball::ChildrenSeekManager::Parametor();
			auto offsets = {
				Vec3(0.0f, 0.0f, +0.15f),
				//Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.25f, 0.0f, -0.5f), Vec3(-0.25f, 0.0f, -0.5f),
			};

			const Col4 Color(0.0f, 1.0f, 0.0f, 0.5f);
			std::vector<Metaball::ChildrenRender::Parametor> childrenParams = {
				Metaball::ChildrenRender_Parametor(1.0f, Color),
				//Metaball::ChildrenRender_Parametor(0.5f, Color),
				Metaball::ChildrenRender_Parametor(0.5f, Color),
				Metaball::ChildrenRender_Parametor(0.5f, Color),
			};

			auto factory = Metaball::Factory_Children(GetStage());
			auto children = factory.Creates(childrenParams);

			for (auto& offset : offsets) {
				const float Speed = 20.0f;
				const float Range = 0.0f;
				auto newOffset = offset;
				const auto SeekType = SeekTarget::SeekType::Lerp;
				parametor.seekParams.push_back(SeekTarget::Parametor(Speed, Range, base + newOffset, SeekType, true));
			}

			//SlimeRenderの設定
			auto render = AddComponent<SlimeRender>();
			//チルドレンを追加。
			render->AddChildren(children);
			//ChildrenのParentを設定
			render->SetChildrenParent(GetThis<GameObject>());
			//Metaball設定
			AddComponent<Metaball::ChildrenSeekManager>(parametor);
			AddComponent<Metaball::ChildrenPressedManager>();

			transform->SetScale(Vec3(1.5f));
			SetAlphaActive(true);
		}
	}
}