/*!
@file SettingStartTarget.cpp
@brief SettingStartTarget�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Utility/Component/Targeted.h"
#include "Maruyama/Utility/Timer/GameTimer.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"

#include "Maruyama/Enemy/AIDirector/CoordinatorBase.h"
#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

#include "Maruyama/Utility/Random.h"

#include "OutTargetTimer.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// �^�[�Q�b�g�����E�O�Ɉ�莞�Ԃ���ƁA�����I��������f�R���[�^�̃p�����[�^
				//--------------------------------------------------------------------------------------

				OutTargetTimer_Parametor::OutTargetTimer_Parametor() :
					OutTargetTimer_Parametor(EyeSearchRangeParametor())
				{}

				OutTargetTimer_Parametor::OutTargetTimer_Parametor(const EyeSearchRangeParametor& eyeParametor) :
					OutTargetTimer_Parametor(eyeParametor, 10.0f, 15.0f)
				{}

				OutTargetTimer_Parametor::OutTargetTimer_Parametor(
					const EyeSearchRangeParametor& eyeParametor,
					const float minLostIntervalTime,
					const float maxLostIntervalTime
				) :
					OutTargetTimer_Parametor(eyeParametor, minLostIntervalTime, maxLostIntervalTime, 30.0f)
				{}

				OutTargetTimer_Parametor::OutTargetTimer_Parametor(
					const EyeSearchRangeParametor& eyeParametor,
					const float minLostIntervalTime,
					const float maxLostIntervalTime,
					const float farRange
				) :
					eyeParametor(eyeParametor),
					minLostIntervalTime(minLostIntervalTime),
					maxLostIntervalTime(maxLostIntervalTime),
					farRange(farRange)
				{}

				//--------------------------------------------------------------------------------------
				/// �^�[�Q�b�g�����E�O�Ɉ�莞�Ԃ���ƁA�����I��������f�R���[�^
				//--------------------------------------------------------------------------------------

				OutTargetTimer::OutTargetTimer(const std::shared_ptr<Enemy::EnemyBase>& owner, const Parametor* paramPtr):
					DecoratorBase(owner),
					m_timer(new GameTimer(0.0f)),
					m_paramPtr(paramPtr)
				{
					auto object = owner->GetGameObject();

					m_eyeRange = object->GetComponent<EyeSearchRange>(false);
					m_targetManager = object->GetComponent<TargetManager>(false);
				}

				void OutTargetTimer::OnStart() {
					m_timer->ResetTimer(GetRandomIntervalTime());
				}

				bool OutTargetTimer::CanUpdate() {
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
							//�^�[�Q�b�g�̐؂�ւ���ʒm����B
							LostTarget();
							return false;
						}
					}
					else {
						m_timer->ResetTimer(GetRandomIntervalTime());
					}

					return true;
				}

				float OutTargetTimer::GetRandomIntervalTime() const {
					return maru::MyRandom::Random(m_paramPtr->minLostIntervalTime, m_paramPtr->maxLostIntervalTime);
				}

				bool OutTargetTimer::IsFarRange() const {
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return true;	//�^�[�Q�b�g�����݂��Ȃ��Ȃ�true
					}

					auto toTargetRange = targetManager->CalcuToTargetVec().length();

					return m_paramPtr->farRange < toTargetRange;	//farRange���^�[�Q�b�g�������ɂ���Ȃ�true
				}

				bool OutTargetTimer::IsLost() const {
					auto targetManager = m_targetManager.lock();
					if (!targetManager || !targetManager->HasTarget()) {
						return true;	//�^�[�Q�b�g�����݂��Ȃ��Ȃ�true
					}

					if (m_eyeRange.lock()->IsInEyeRange(targetManager->GetTarget(), m_paramPtr->eyeParametor)) {
						return false;	//�^�[�Q�b�g�����E���ɂ���Ȃ�false(Lost���ĂȂ�)
					}

					return true;		//���E���ɂ��Ȃ����߁ALost
				}

				void OutTargetTimer::LostTarget() {
					using namespace Enemy;

					auto assignedFacntion = GetOwner()->GetAssignedFaction();
					if (!assignedFacntion) {
						return;
					}

					auto tupleSpace = assignedFacntion->GetTupleSpace();

					//�^�[�Q�b�g���X�g��`����B
					tupleSpace->Write<Tuple::LostTarget>(
						GetOwner(),
						m_targetManager.lock(),
						0.0f
					);

					//�^�[�Q�b�g�̌����𗊂�
					tupleSpace->Write<Tuple::SearchTarget>(
						GetOwner(),
						m_targetManager.lock(),
						0.0f
					);
				}

			}
		}
	}
}