/*!
@file BigSlimeBase.cpp
@brief BigSlimeBase�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"


#include "EnemyBase.h"
#include "I_KnockBack.h"
#include "I_PushedWater.h"
#include "SlimeBase.h"
#include "BigSlimeBase.h"

#include "EnemyMainStateMachine.h"
#include "StatorBase.h"
#include "SlimeStatorBase.h"
#include "Stator_BigSlime.h"

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

#include "Pusher.h"
#include "TimeHelper.h"
#include "InvolvedManager.h"
#include "MetaballRenderBase.h"
#include "PressCenterManager.h"
#include "MetaballChildrenPressedManager.h"
#include "UtilityVelocity.h"
#include "SingletonComponent.h"
#include "EffectManager.h"

#include "SlimeStatorBase.h"

#include "I_KnockBack.h"
#include "MaruAnimationHelper.h"
#include "TaskList.h"
#include "MetaballMotionManager.h"
#include "Motion_KnockBack.h"

namespace basecross {
	namespace Enemy {

		namespace BigSlime {

			BigSlimeBase::BigSlimeBase(const std::shared_ptr<GameObject>& objPtr):
				SlimeBase(objPtr)
			{}

			//--------------------------------------------------------------------------------------
			/// �C���^�[�t�F�[�X�̎���
			//--------------------------------------------------------------------------------------

			PressedEnergy BigSlimeBase::Pressed(const PressData& pressData) {
				//�X�e�[�g�̐���
				auto pressManager = GetGameObject()->GetComponent<Metaball::ChildrenPressedManager>(false);

				//�������g��Press��ԂɕύX
				if (auto stator = GetGameObject()->GetComponent<I_Stator_EnumType<Enemy::SlimeStateType>>(false)) {
					stator->ChangeState(Stator::State::Pressed, int(Stator::State::Pressed));
				}

				//�������݃I�u�W�F�N�g�Ȃ�q�I�u�W�F�N�g���ƂԂ��B
				InvolvedPressed();

				return SlimeBase::Pressed(pressData);
			}

			void BigSlimeBase::InvolvedPressed() {
				auto pressCenterManager = GetGameObject()->GetComponent<PressCenterManager>(false);
				auto absorbedManager = GetGameObject()->GetComponent<BigSlime::AbsorbedSlimeManagerBase>(false);
				if (!pressCenterManager || !absorbedManager) { //���ꂼ��̃f�[�^��nullCheck
					return;
				}

				const auto pressData = pressCenterManager->GetSuccessPressData();
				float pressPower = GetGameObject()->GetComponent<Metaball::ChildrenPressedManager>(false)->GetPressedPower();

				//�������݃I�u�W�F�N�g�o�Ȃ������珈�����΂��B
				auto involved = pressData.GetPusherObjectComponent<InvolvedManager>();
				if (!involved) {
					return;
				}

				//�z�����̃X���C���̉��
				for (auto& object : absorbedManager->GetAbsorbedObjects()) {
					if (auto pressCenterManager = object->GetComponent<PressCenterManager>(false)) {
						pressCenterManager->SetSuccessPressData(pressData);
					}

					if (auto stator = object->GetComponent<I_Stator_EnumType<Enemy::SlimeStateType>>(false)) {
						stator->ChangeState(SlimeStateType::Pressed);
					}
				}
			}


			void BigSlimeBase::Spread(const PressData& pressData) {
				auto absorbedManager = GetGameObject()->GetComponent<BigSlime::AbsorbedSlimeManagerBase>(false);
				auto childrenPressManager = GetGameObject()->GetComponent<Metaball::ChildrenPressedManager>(false);
				if (!absorbedManager || !childrenPressManager) {
					DebugObject::AddString(L"BigSlimeBase::Spread, �K�v�R���|�[�l���g�����݂��܂���B");
					return;
				}

				//�z�����̃I�u�W�F�N�g���Ȃ��Ȃ�
				if (absorbedManager->GetNumAbsrobedObjects() == 0) {
					SlimeBase::Spread(pressData);
					return;
				}

				//������RederChildRen���폜
				if (auto render = GetGameObject()->GetComponent<Metaball::RenderBase>(false)) {
					render->AllReleaseChildren();
				}

				//�z�����̃X���C���̉��
				for (auto& object : absorbedManager->GetAbsorbedObjects()) {
					//�e�I�u�W�F�N�g�̉��
					auto objTrans = object->GetComponent<Transform>();
					auto worldPosition = objTrans->GetWorldPosition();
					object->SetParent(nullptr);
					objTrans->SetWorldPosition(worldPosition);

					//�m�b�N�o�b�N�X�e�[�g�ɕύX
					if (auto stator = object->GetComponent<I_Stator_EnumType<SlimeStateType>>(false)) {
						stator->ChangeState(SlimeStateType::KnockBack, (int)SlimeStateType::KnockBack);
					}

					//�z�������X���C����Draw�̕���
					if (auto render = object->GetComponent<Metaball::RenderBase>(false)) {
						render->SetDrawActive(true);
					}

					//������΂�����
					auto childPosition = object->GetComponent<Transform>()->GetPosition();

					auto velocityManager = object->GetComponent<VelocityManager>();
					auto velocity = CalculateSpreadVelocity(GetGameObject(), object.GetShard(), pressData);

					constexpr float BaseAdjustVelocityPower = 5.0f;
					constexpr float DeselerationPower = 0.5f;
					auto adjustVelocityPower = BaseAdjustVelocityPower * (float)absorbedManager->GetNumAbsrobedObjects();
					velocityManager->SetVelocity(velocity * (float)absorbedManager->GetNumAbsrobedObjects());
					velocityManager->StartDeseleration(DeselerationPower);

					//�p�[�e�B�N���̐���
					if (auto effectManager = EffectManager::GetInstance()) {
						effectManager->Play(EffectManager::ID::SlimeCrush, childPosition);
					}
				}
			}

			Motion::KnockBack_Parametor BigSlimeBase::GetPressedMotionData(const PressData& pressData) {
				auto resultData = SlimeBase::GetPressedMotionData(pressData);

				auto absorbed = GetGameObject()->GetComponent<AbsorbedSlimeManagerBase>(false);
				if (!absorbed) {
					return resultData;
				}

				auto magnification = static_cast<float>(absorbed->GetNumAbsrobedObjects());
				resultData.data.power *= magnification;
				resultData.range *= magnification;
				resultData.maxRangeVec *= magnification;
				resultData.transitionTime /= magnification;

				return resultData;
			}

		}
	}
}