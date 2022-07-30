
/*!
@file BigSlimeObject.cpp
@brief BigSlimeObject�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyObjectBase.h"
#include "SlimeObjectBase.h"
#include "BigSlimeObject.h"

#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_BigSlime.h"

#include "AnimatorBase.h"
#include "MaruAnimationHelper.h"
#include "AnimationMainStateMachine.h"
#include "Animator_NormalSlime.h"

#include "I_KnockBack.h"
#include "I_PushedWater.h"
#include "SlimeBase.h"
#include "BigSlimeBase.h"
#include "BigNormalSlime.h"

#include "AbsorbedSlimeManager.h"

#include "PressCenterManager.h"

#include "MetaballChildrenPressedManager.h"

#include "EnemyObjectBase.h"
#include "NormalSlimeObject.h"

#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_NormalSlime.h"

namespace basecross {
	namespace Enemy {
		namespace BigSlime {

			BigSlimeObject::BigSlimeObject(const std::shared_ptr<Stage>& stage)
				:SlimeObjectBase(stage)
			{}

			void BigSlimeObject::OnCreate() {
				AddComponent<BigNormalSlime>();
				SlimeObjectBase::OnCreate();

				//�X�e�[�^�[
				AddComponent<Stator>();

				//�z��Slime�Ǘ��̃R���|�[�l���g
				auto param = AbsorbedSlimeManagerBase::Parametor();
				param.createDatas.push_back(CreateSlimeObjectData(Vec3(+1.5f, 0.0, 0.0f)));
				param.createDatas.push_back(CreateSlimeObjectData(Vec3(-1.5f, 0.0, 0.0f)));
				auto absorbedMangaer = AddComponent<AbsorbedSlimeManager<NormalSlimeObject>>(param);  
				absorbedMangaer->ChangeStates<Stator_NormalSlime>(Stator_NormalSlime::State::Absorbed);
				//�����蔻��̒���
				GetComponent<CollisionObb>()->SetMakedSize((float)absorbedMangaer->GetAbsorbedObjects().size());
			}

			void BigSlimeObject::CreateModel() {
				AddComponent<Metaball::ChildrenPressedManager>();
			}

			void BigSlimeObject::SettingScalePop() {

			}

		}
	}
}


