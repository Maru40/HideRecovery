
/*!
@file SlimeBase.cpp
@brief SlimeBase�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "I_PushedWater.h"
#include "I_Pressed.h"
#include "PressData.h"
#include "SlimeBase.h"

#include "SeekTarget.h"
#include "MetaballChildrenRender.h"
#include "MetaballChildrenPressedManager.h"
#include "MetaballRenderBase.h"
#include "SlimeRender.h"

#include "EnemyStatusBase.h"

#include "I_KnockBack.h"
#include "VelocityManager.h"

#include "SingletonComponent.h"
#include "EffectManager.h"

#include "TimeHelper.h"
#include "UtilityVelocity.h"

#include "PressCenterManager.h"

#include "I_KnockBack.h"
#include "MaruAnimationHelper.h"
#include "TaskList.h"
#include "MetaballMotionManager.h"
#include "Motion_KnockBack.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �p�[�e�B�N���̐���
		//--------------------------------------------------------------------------------------
		void CreateParticle(const Vec3& position) {
			auto effectManager = EffectManager::GetInstance();
			if (effectManager) {
				effectManager->Play(EffectManager::ID::SlimeCrush, position);
			}
		}

		//--------------------------------------------------------------------------------------
		/// �X���C���̊��N���X�{��
		//--------------------------------------------------------------------------------------

		SlimeBase::SlimeBase(const std::shared_ptr<GameObject>& objPtr)
			:EnemyBase(objPtr)
		{}

		void SlimeBase::OnCreate() {

		}

		Vec3 SlimeBase::CalculateSpreadVelocity(
			const std::shared_ptr<GameObject>& selfObject,
			const std::shared_ptr<GameObject>& childObject,
			const PressData& pressData
		) {
			auto childPosition = childObject->GetComponent<Transform>()->GetPosition();
			auto pressManager = selfObject->GetComponent<Metaball::ChildrenPressedManager>(false);
			auto velocityManager = childObject->GetComponent<VelocityManager>(false);

			//�K�v�ȏ��Q��NullCheck  Pusher, pressCenterManager, velocityManager
			if (!pressManager || !velocityManager) {
				DebugObject::AddString(L"SlimeBase::Spread()", L"�K�v�ȃR���|�[�l���g������܂���B");
				return Vec3(0.0f);
			}

			auto selfToChild = childPosition - transform->GetPosition();
			auto pusherToChild = childPosition - pressData.GetPusherPosition();

			//������͂��v�Z
			float delta = maru::TimeHelper::GetElapsedTime(maru::DeltaType::Real);
			auto basePower = pressData.CalculatePressPower();
			auto power = basePower * pressManager->GetPressedPower();

			//������������v�Z
			auto direct = childPosition - selfObject->GetComponent<Transform>()->GetPosition();
			direct = UtilityVelocity::Reflection(direct, pressData.collisionPair.m_SrcHitNormal); //direct��ǂɍ��킹�Ĕ��˂�����B
			auto resultVelocity = direct.GetNormalized() * power * delta;

			return resultVelocity;
		}

		void SlimeBase::AddSpreadPower(
			const std::shared_ptr<Metaball::RenderBase>& render,
			const std::shared_ptr<Metaball::ChildrenRender>& child,
			const PressData& pressData
		) {
			auto childPosition = child->GetGameObject()->GetComponent<Transform>()->GetPosition();
			auto pressManager = render->GetGameObject()->GetComponent<Metaball::ChildrenPressedManager>(false);
			auto velocityManager = child->GetGameObject()->GetComponent<VelocityManager>(false);

			//�K�v�ȏ��Q��NullCheck  Pusher, pressCenterManager, velocityManager
			if (pressManager && velocityManager) {
				auto velocity = CalculateSpreadVelocity(render->GetGameObject(), child->GetGameObject(), pressData);
				//�X�s�[�h���Z�b�g
				velocityManager->SetVelocity(velocity);
			}
		}

		//--------------------------------------------------------------------------------------
		/// �C���^�[�t�F�[�X�̎���
		//--------------------------------------------------------------------------------------

		PressedEnergy SlimeBase::Pressed(const PressData& pressData) {
			//ChildrenPressedManager��Press����B
			if (auto pressedManager = GetGameObject()->GetComponent<Metaball::ChildrenPressedManager>(false)) {
				pressedManager->PressCrash(pressData);
			}

			//PressCenterManager�Œׂ�������ʒm
			if (auto pressCenterManager = GetGameObject()->GetComponent<PressCenterManager>(false)) {
				pressCenterManager->SetSuccessPressData(pressData);
			}

			//���x�̃��Z�b�g
			if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
				velocityManager->ResetAll();
			}

			//�X�e�[�^�X�Ƀf�[�^��n���B
			auto status = GetGameObject()->GetComponent<EnemyStatusManagerBase>(false);
			return status ? status->GetStatus().pressedEnergy : PressedEnergy(0.0f);
		}

		void SlimeBase::Push(const PushData& data) {
			if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
				auto direction = data.direction;
				velocityManager->AddForce(direction.GetNormalized() * data.power);
			}

			if (auto knockBack = GetGameObject()->GetComponent<I_KnockBack>(false)) {
				knockBack->KnockBack(KnockBackData(data.power, data.hitPoint, data.direction));
			}
		}

		void SlimeBase::Spread(const PressData& pressData) {
			auto render = GetGameObject()->GetComponent<Metaball::RenderBase>(false);

			for (auto& child : render->GetChildren()) {
				if (!child) { //NullCheck
					continue;
				}

				//�������Ȃ��č폜�����B
				constexpr float Speed = 1.0f;
				child->StartScaleFade(Speed);

				//�Ǐ]��Ԃ�����
				if (auto seek = child->GetGameObject()->GetComponent<SeekTarget>(false)) {
					seek->SetUpdateActive(false);
				}

				//�g�U�����ɗ͂�������B
				AddSpreadPower(render, child.GetShard(), pressData);

				//�p�[�e�B�N���̐���
				auto childPosition = child->GetGameObject()->GetComponent<Transform>()->GetPosition();
				CreateParticle(childPosition);
			}
		};

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		Motion::KnockBack_Parametor SlimeBase::GetPressedMotionData(const PressData& pressData) {
			Motion::KnockBack_Parametor resultParam;

			KnockBackData data;
			data.hitPoint = pressData.collisionPair.m_CalcHitPoint;
			data.direct = pressData.pressedVec.GetNormalized();
			data.power = pressData.pressedVec.length();
			resultParam.range = 1.5f;
			resultParam.transitionTime = 0.075f;
			resultParam.data = data;
			resultParam.maxRangeVec = Vec3(0.55f, 0.25f, 0.55f);
			resultParam.avoidRange *= 2.0f;

			return resultParam;
		}

	}
}