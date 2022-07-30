
/*!
@file Slime_Find.cpp
@brief Slime_Find�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyBase.h"
#include "EnemyStateNodeBase.h"
#include "Slime_Union.h"

#include "TimeHelper.h"
#include "GameTimer.h"

#include "TargetManager.h"


#include "MetaballRenderBase.h"
#include "SlimeRender.h"

#include "MaruAction.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Slime_Union::Slime_Union(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr) :
				EnemyStateNodeBase(owner), m_paramPtr(paramPtr), m_timer(new GameTimer(0.0f))
			{}

			void Slime_Union::OnStart() {
				m_timer->ResetTimer(m_paramPtr->time); 

				//�J�n�ʒu���L�^
				m_startPosition = GetOwner()->GetGameObject()->GetComponent<Transform>()->GetPosition();

				//�Е���Render�����킹��B
				TranserChildren();
			}

			bool Slime_Union::OnUpdate() {
				m_timer->UpdateTimer();

				//����������
				UpdateUnion();

				return m_timer->IsTimeUp();
			}

			void Slime_Union::OnExit() {
				//�o�ߎ��Ԃ��I�����Ă�����A
				if (m_timer->IsTimeUp()) {
					SccessUnion(); //Union����
				}
				else {	
					MissUnion();   //Union���s
				}
			}

			void Slime_Union::TranserChildren() {
				auto ownerObject = GetOwner()->GetGameObject();
				auto targetManager = ownerObject->GetComponent<TargetManager>(false);
				if (!targetManager) {
					return;
				}

				maru::Action<void()> removeFunc;  //�폜����ŌĂԂ��߂�Anition
				auto selfRender = ownerObject->GetComponent<SlimeRender>(); //������Render
				auto targetRender = targetManager->GetTarget()->GetComponent<SlimeRender>();  //�����Render
				for (auto& child : selfRender->GetChildren()) {
					targetRender->AddChild(child.GetShard()); //Render�Ɏ�����Cube��n��
					removeFunc.AddFunction([&, child]() { selfRender->RemoveChild(child); }); //�������g����Child���폜�C�x���g�̔��s
				}

				removeFunc.Invoke(); //Child�̍폜
			}

			void Slime_Union::UpdateUnion() {
				auto ownerObject = GetOwner()->GetGameObject();
				auto ownerTrans = ownerObject->GetComponent<Transform>();
				auto targetManager = ownerObject->GetComponent<TargetManager>(false);
				if (!targetManager) {
					return;
				}

				const float StartTime = 0.0f;
				const float EndTime = 1.0f;
				auto halfToTargetVec = targetManager->CalcuToTargetVec() * 0.5f;
				auto endPosition = ownerTrans->GetPosition() + halfToTargetVec;

				auto position = Lerp::CalculateLerp(m_startPosition, endPosition, StartTime, EndTime, m_timer->GetTimeRate(), Lerp::rate::Linear);
				ownerTrans->SetPosition(position);
			}

			void Slime_Union::SccessUnion() {

			}

			void Slime_Union::MissUnion() {

			}

		}
	}
}