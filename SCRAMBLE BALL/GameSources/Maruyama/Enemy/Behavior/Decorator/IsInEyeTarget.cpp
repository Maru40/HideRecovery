/*!
@file IsInEyeTargets.cpp
@brief IsInEyeTargets�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "IsInEyeTarget.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/Utility/Timer/GameTimer.h"

#include "Maruyama/Utility/Utility.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// �Ď��Ώۂ����E�͈͂ɂ��邩�ǂ����𔻒f����f�R���[�^�̃p�����[�^
				//--------------------------------------------------------------------------------------

				IsInEyeTarget_Parametor::IsInEyeTarget_Parametor() :
					IsInEyeTarget_Parametor(EyeSearchRangeParametor())
				{}

				IsInEyeTarget_Parametor::IsInEyeTarget_Parametor(const EyeSearchRangeParametor& eyeParametor) :
					IsInEyeTarget_Parametor(eyeParametor, 10.0f)
				{}

				IsInEyeTarget_Parametor::IsInEyeTarget_Parametor(
					const EyeSearchRangeParametor& eyeParametor,
					const float lostIntervalTime
				):
					IsInEyeTarget_Parametor(eyeParametor, lostIntervalTime, 30.0f)
				{}

				IsInEyeTarget_Parametor::IsInEyeTarget_Parametor(
					const EyeSearchRangeParametor& eyeParametor,
					const float lostIntervalTime, 
					const float farRange
				) :
					eyeParametor(eyeParametor),
					lostIntervalTime(lostIntervalTime),
					farRange(farRange)
				{}

				//--------------------------------------------------------------------------------------
				/// �Ď��Ώۂ����E�͈͂ɂ��邩�ǂ����𔻒f����f�R���[�^
				//--------------------------------------------------------------------------------------

				IsInEyeTarget::IsInEyeTarget(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr):
					DecoratorBase(owner),
					m_paramPtr(paramPtr),
					m_timer(new GameTimer(0.0f))
				{
					m_eyeRange = owner->GetGameObject()->GetComponent<EyeSearchRange>(false);
					m_targetManager = owner->GetGameObject()->GetComponent<TargetManager>(false);
				}

				bool IsInEyeTarget::CanTransition() const {
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return false;
					}

					return m_eyeRange.lock()->IsInEyeRange(targetManager->GetTarget(), m_paramPtr->eyeParametor.length);
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
						m_timer->ResetTimer(m_paramPtr->lostIntervalTime);
					}

					return true;
				}

				bool IsInEyeTarget::IsFarRange() const {
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return true;	//�^�[�Q�b�g�����݂��Ȃ��Ȃ�true
					}

					auto toTargetRange = targetManager->CalcuToTargetVec().length();

					return m_paramPtr->farRange < toTargetRange;	//farRange���^�[�Q�b�g�������ɂ���Ȃ�true
				}

				bool IsInEyeTarget::IsLost() const {
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return true;	//�^�[�Q�b�g�����݂��Ȃ��Ȃ�true
					}

					auto targetPosition = targetManager->GetTargetPosition();
					if (m_eyeRange.lock()->IsInEyeRange(targetPosition), m_paramPtr->eyeParametor.length) {
						return false;	//�^�[�Q�b�g�����E���ɂ���Ȃ�false(Lost���ĂȂ�)
					}

					return true;		//���E���ɂ��Ȃ����߁ALost
				}

			}
		}
	}
}