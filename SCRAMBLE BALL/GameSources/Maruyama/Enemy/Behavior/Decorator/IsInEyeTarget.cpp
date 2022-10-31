/*!
@file IsInEyeTarget.cpp
@brief IsInEyeTarget�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "IsInEyeTarget.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/Utility/Timer/GameTimer.h"
#include "Maruyama/Utility/ObserveIsInEyeTarget.h"

#include "Maruyama/Utility/Utility.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// �Ď��Ώۂ����E�͈͂ɂ��邩�ǂ����𔻒f����f�R���[�^�̃p�����[�^
				//--------------------------------------------------------------------------------------

				IsInEyeTarget_Parametor::IsInEyeTarget_Parametor():
					IsInEyeTarget_Parametor(10.0f, 20.0f)
				{}

				IsInEyeTarget_Parametor::IsInEyeTarget_Parametor(const float lostIntervalTime, const float farRange):
					lostIntervalTime(lostIntervalTime),
					farRange(farRange)
				{}

				//--------------------------------------------------------------------------------------
				/// �Ď��Ώۂ����E�͈͂ɂ��邩�ǂ����𔻒f����f�R���[�^�{��
				//--------------------------------------------------------------------------------------

				IsInEyeTarget::IsInEyeTarget(const std::shared_ptr<Enemy::EnemyBase>& owner) :
					IsInEyeTarget(owner, std::vector<std::weak_ptr<GameObject>>())
				{}

				IsInEyeTarget::IsInEyeTarget(
					const std::shared_ptr<Enemy::EnemyBase>& owner,
					const ObserveSharedTargets& observeTargets,
					const Parametor& parametor
				):
					IsInEyeTarget(
						owner, 
						maru::Utility::ConvertArraySharedToWeak(observeTargets),
						parametor
					)
				{}

				IsInEyeTarget::IsInEyeTarget(
					const std::shared_ptr<Enemy::EnemyBase>& owner,
					const ObserveTargets& observeTargets,
					const Parametor& parametor
				):
					DecoratorBase(owner),
					m_param(parametor),
					m_timer(new GameTimer(0.0f)),
					m_observeIsInTarget(new ObserveIsInEyeTarget(owner->GetGameObject()->GetComponent<EyeSearchRange>(), observeTargets))
				{
					m_eyeRange = owner->GetGameObject()->GetComponent<EyeSearchRange>(false);
					m_targetManager = owner->GetGameObject()->GetComponent<TargetManager>(false);
				}

				bool IsInEyeTarget::CanTransition() const {
					auto target = m_observeIsInTarget->SearchIsInEyeTarget();
					
					return target ? true : false;	//�^�[�Q�b�g�����݂���Ȃ�true
				}

				bool IsInEyeTarget::CanUpdate() {
					//�^�[�Q�b�g�����݂��Ȃ��Ȃ�A�X�V�ł��Ȃ��B
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return false;
					}

					//�^�[�Q�b�g�����Ȃ艓���ɂ���Ȃ�A�Ǐ]��������߂�B
					if (IsFarRange()) {
						return false;
					}

					//�^�[�Q�b�g���������āA��莞�Ԃ�������AUpdate���I��
					if (IsLost()) {
						//����������Ɉ�莞�Ԃ�������
						m_timer->UpdateTimer();
						if (m_timer->IsTimeUp()) {
							return false;
						}
					}
					else {
						m_timer->ResetTimer(m_param.lostIntervalTime);
					}

					return true;
				}

				bool IsInEyeTarget::IsLost() const {
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return true;	//�^�[�Q�b�g�����݂��Ȃ��Ȃ�true
					}

					auto targetPosition = targetManager->GetTargetPosition();
					if (m_eyeRange.lock()->IsInEyeRange(targetPosition)) {
						return false;	//�^�[�Q�b�g�����E���ɂ���Ȃ�false(Lost���ĂȂ�)
					}

					return true;		//���E���ɂ��Ȃ����߁ALost
				}

				bool IsInEyeTarget::IsFarRange() const {
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return true;	//�^�[�Q�b�g�����݂��Ȃ��Ȃ�true
					}

					auto toTargetRange = targetManager->CalcuToTargetVec().length();

					return m_param.farRange < toTargetRange;	//farRange���^�[�Q�b�g�������ɂ���Ȃ�true
				}

				//--------------------------------------------------------------------------------------
				/// �A�N�Z�b�T
				//--------------------------------------------------------------------------------------

				void IsInEyeTarget::AddObserveTarget(const std::shared_ptr<GameObject>& target) {
					m_observeIsInTarget->AddObserveTarget(target);
				}

				void IsInEyeTarget::SetObserveTargets(const ObserveTargets& targets) {
					m_observeIsInTarget->SetObserveTargets(targets); 
				}

				void IsInEyeTarget::SetObserveTargets(const ObserveSharedTargets& targets) {
					SetObserveTargets(maru::Utility::ConvertArraySharedToWeak(targets));
				}

				_NODISCARD IsInEyeTarget::ObserveTargets IsInEyeTarget::GetObserveTargets() const noexcept {
					return m_observeIsInTarget->GetObserveTargets();
				}

			}
		}
	}
}