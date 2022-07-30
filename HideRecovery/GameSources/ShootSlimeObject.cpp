/*!
@file ShootSlimeObject.cpp
@brief ShootSlimeObject�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyObjectBase.h"
#include "SlimeObjectBase.h"

#include "SeekTarget.h"

#include "ShootSlimeObject.h"

#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_ShootSlime.h"

#include "SlimeCore.h"
#include "MetaballRenderBase.h"
#include "SlimeRender.h"

#include "ShootManager.h"

#include "GameTimer.h"
#include "TaskList.h"
#include "SeekTargetOffsetsManager.h"
#include "MetaballChildrenSeekManager.h"
#include "MetaballChildrenPressedManager.h"

#include "I_KnockBack.h"
#include "I_PushedWater.h"
#include "SlimeBase.h"
#include "ShootSlime.h"

#include "AnimationMainStateMachine.h"
#include "AnimatorBase.h"
#include "Animator_ShootSlime.h"

#include "MetaballMotionManager.h"

#include "MetaballRenderBase.h"
#include "SlimeRender.h"

#include "MetaballChildrenRender.h"

namespace basecross {
	namespace Enemy {

		namespace ShootSlime {

			ShootSlimeObject::ShootSlimeObject(const std::shared_ptr<Stage>& stage)
				:SlimeObjectBase(stage)
			{}

			void ShootSlimeObject::OnCreate() {
				AddComponent<ShootSlime>();
				SlimeObjectBase::OnCreate();

				AddComponent<Animator>();
				AddComponent<Stator_ShootSlime>();
				AddComponent<ShootManager>();
			}

			void ShootSlimeObject::CreateModel() {
				//Core
				const auto Color = Col4(0.1f, 0.1f, 0.8f, 1.0f);
				auto core = AddComponent<SlimeCore>();
				core->SetEyeDiffuse(Color);
				//SlimeRender
				Vec3 basePosition(0.0f, 0.45f, 0.0f);
				auto parametor = Metaball::ChildrenSeekManager::Parametor();
				auto offsets = {
					Vec3(0.0f, -0.0f, 0.0f),

					Vec3(0.75f, -0.75f, 0.0f), Vec3(-0.75f, -0.75f, 0.0f),
					Vec3(0.0f, -0.75f, -0.0f),
				};

				std::vector<Metaball::ChildrenRender::Parametor> childrenParams = {
					Metaball::ChildrenRender_Parametor(1.0f, Color),
					Metaball::ChildrenRender_Parametor(0.5f, Color),
					Metaball::ChildrenRender_Parametor(0.5f, Color),
				};
				auto factory = Metaball::Factory_Children(GetStage());
				auto children = factory.Creates(childrenParams);

				for (const auto& offset : offsets) {
					const float Speed = 20.0f;
					const float Range = 0.0f;
					const auto SeekType = SeekTarget::SeekType::Lerp;
					parametor.seekParams.push_back(SeekTarget::Parametor(Speed, Range, basePosition + offset, SeekType, true));
				}

				const int NumCreate = static_cast<int>(offsets.size());
				//SlimeRender�̐ݒ�
				auto render = AddComponent<SlimeRender>();
				//�`���h������ǉ��B
				render->AddChildren(children);
				//Parent�ݒ�
				render->SetChildrenParent(GetThis<GameObject>());
				//Metaball�ݒ�
				AddComponent<Metaball::ChildrenSeekManager>(parametor);
				AddComponent<Metaball::ChildrenPressedManager>();

				transform->SetScale(Vec3(1.25f));
				SetAlphaActive(true);
			}

		}
	}
}