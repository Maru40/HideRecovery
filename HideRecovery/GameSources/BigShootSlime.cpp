/*!
@file BigShootSlime.cpp
@brief BigShootSlime�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyBase.h"
#include "I_KnockBack.h"
#include "I_PushedWater.h"
#include "SlimeBase.h"
#include "BigSlimeBase.h"
#include "BigShootSlime.h"

#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_BigShootSlime.h"

#include "TaskList.h"
#include "SeekTarget.h"
#include "MaruAnimationHelper.h"
#include "MetaballChildrenObject.h"
#include "MetaballChildrenRender.h"
#include "MetaballMotionManager.h"

#include "I_BrackBoard.h"
#include "AnimationMainStateMachine.h"
#include "AnimatorBase.h"
#include "Animator_BigSlime.h"

#include "I_KnockBack.h"

#include "Motion_KnockBack.h"

#include "EnemyStatusBase.h"

#include "PressData.h"
#include "VelocityManager.h"
#include "Motion_KnockBack.h"
#include "Slime_Dyning.h"
#include "MetaballChildrenPressedManager.h"

#include "AbsorbedSlimeManager.h"

#include "EnemyObjectBase.h"
#include "SlimeObjectBase.h"

#include "BigSlime_Dyning.h"

#include "Animator_NormalSlime.h"
#include "Stator_NormalSlime.h"
#include "Stator_ShootSlime.h"
#include "Animator_ShootSlime.h"

#include "EnemyObjectBase.h"

namespace basecross {
	namespace Enemy {

		namespace BigShootSlime {

			BigShootSlime::BigShootSlime(const std::shared_ptr<GameObject>& objPtr)
				:BigSlimeBase(objPtr)
			{}

			//--------------------------------------------------------------------------------------
			/// �C���^�[�t�F�[�X�̎���
			//--------------------------------------------------------------------------------------

			void BigShootSlime::KnockBack(const KnockBackData& data) {
				auto statusManager = GetGameObject()->GetComponent<EnemyStatusManagerBase>(false);
				if (statusManager && !statusManager->IsToughnessBreak(data.power)) {
					return;
				}

				//�z���I�u�W�F�N�g�ɓ`����B
				auto absorbedManager = GetGameObject()->GetComponent<BigSlime::AbsorbedSlimeManagerBase>(false);
				if (absorbedManager) {
					for (auto& object : absorbedManager->GetAbsorbedObjects()) {
						if (!object) {
							DebugObject::AddString(L"BigShootSlime::KnockBack, �z�������X���C����Metaball��null�ł��B");
							continue;
						}

						//�A�j���[�V�����̍Đ�
						auto animator = object->GetComponent<ShootSlime::Animator>(false);
						if (animator) {
							auto& blackBoard = animator->GetRefBlackBoard();
							blackBoard.knockBackParamPtr->data = data;

							animator->ChangeState(ShootSlime::Animator::StateType::KnockBack);
						}
					}
				}

				//�������g�̃X�e�[�g��ύX
				auto stator = GetGameObject()->GetComponent<Stator>(false);
				if (stator) {
					stator->GetTransitionMember().knockBackTrigger.Fire();
				}
			}

			PressedEnergy BigShootSlime::Pressed(const PressData& pressData) {
				//�z���I�u�W�F�N�g�ɓ`����B
				auto absorbedManager = GetGameObject()->GetComponent<BigSlime::AbsorbedSlimeManagerBase>(false);
				if (absorbedManager) {
					for (auto& object : absorbedManager->GetAbsorbedObjects()) {
						//�A�j���[�V�����̍Đ�
						auto animator = object->GetComponent<ShootSlime::Animator>(false);
						auto velocityManager = object->GetComponent<VelocityManager>(false);
						if (animator && velocityManager) {
							//�A�j���[�^�̃u���b�N�{�[�h�Ƀf�[�^��`����B
							auto& blackBoard = animator->GetRefBlackBoard();
							*blackBoard.pressedParamPtr = GetPressedMotionData(pressData);

							animator->ChangeState(ShootSlime::Animator_StateType::KnockBack, (int)NormalSlime::Animator_StateType::Max);
						}
					}
				}

				return BigSlimeBase::Pressed(pressData);
			}

		}
	}
}