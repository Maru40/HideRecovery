/*!
@file TimerWaitPosition.cpp
@brief TimerWaitPosition�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "TimerWaitPosition.h"

#include "Maruyama/Utility/Timer/GameTimer.h"
#include "Maruyama/Interface/I_FactionMember.h"
#include "Maruyama/Enemy/AIDirector/CoordinatorBase.h"
#include "Maruyama/Enemy/AIDirector/FactionCoordinator.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				//--------------------------------------------------------------------------------------
				/// ��莞�ԓ����ꏊ�ɂƂǂ܂��Ă������x�^�X�N�����Z�b�g���邽�߂ɁA�߂�f�R���[�^�̃p�����[�^
				//--------------------------------------------------------------------------------------

				TimerWaitPosition_Parametor::TimerWaitPosition_Parametor():
					time(3.0f),
					updateRange(2.0f)
				{}

				//--------------------------------------------------------------------------------------
				/// ��莞�ԓ����ꏊ�ɂƂǂ܂��Ă������x�^�X�N�����Z�b�g���邽�߂ɁA�߂�f�R���[�^
				//--------------------------------------------------------------------------------------

				TimerWaitPosition::TimerWaitPosition(const std::shared_ptr<GameObject>& owner) :
					DecoratorBase(owner),
					m_param(Parametor()),
					m_timer(new GameTimer(m_param.time))
				{
					m_transform = owner->GetComponent<Transform>();
				}

				void TimerWaitPosition::OnStart() {
					UpdatePosition();
					m_timer->ResetTimer(m_param.time);
				}

				bool TimerWaitPosition::CanUpdate()  {
					m_timer->UpdateTimer();

					//�o�^���ꂽ�|�W�V��������苗�����ꂽ��A�|�W�V�������X�V�A���Ԃ��X�V
					if (IsUpdatePosition()) {
						UpdatePosition();
						m_timer->ResetTimer(m_param.time);
					}

					bool isTimeUp = m_timer->IsTimeUp();
					if (isTimeUp) {
						//�����Ƀ^�[�Q�b�g�˗��s��������Ƃ��������H
						//auto factionMember = GetOwner()->GetComponent<Enemy::I_FactionMember>(false);
						//if (factionMember) {
						//	auto faction = factionMember->GetFactionCoordinator();
						//	if (faction) {
						//		//�^�[�Q�b�g��ʂɂ��Ă��炤�悤�ɂ��肢����B
						//	}
						//}

						Debug::GetInstance()->Log(L"TimeUp: ��莞�ԓ����ꏊ�ɂ��܂����B");
					}

					return !isTimeUp;	//���Ԃ��I��������A�X�V�������I����B
				}

				void TimerWaitPosition::UpdatePosition() {
					m_observePosition = m_transform.lock()->GetPosition();
				}

				bool TimerWaitPosition::IsUpdatePosition() {
					auto toObservePosition = m_observePosition - m_transform.lock()->GetPosition();
					auto toRange = toObservePosition.length();	//�������擾

					return m_param.updateRange < toRange;	//�ݒ苗����藣�ꂽ��Atrue
				}
			}
		}
	}
}