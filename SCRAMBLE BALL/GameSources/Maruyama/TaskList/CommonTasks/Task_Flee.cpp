/*!
@file Task_Flee.cpp
@brief Task_Fleeのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Task_Flee.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Utility/UtilityVelocity.h"
#include "VelocityManager.h"
#include "Maruyama/Utility/Component/RotationController.h"

namespace basecross {
	namespace Enemy {

		namespace Task {

			//パラメータ----------------------------------------------------------------------------------

			Flee_Parametor::Flee_Parametor()
				:Flee_Parametor(3.0f)
			{}

			Flee_Parametor::Flee_Parametor(const float& maxSpeed)
				: Flee_Parametor(maxSpeed, 0.0f, false)
			{}

			Flee_Parametor::Flee_Parametor(const float& maxSpeed, const float& time, const bool isTimer)
				:maxSpeed(maxSpeed), time(time), isTimer(isTimer)
			{}

			//--------------------------------------------------------------------------------------------

			Flee::Flee(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
				:TaskNodeBase<EnemyBase>(owner), m_paramPtr(paramPtr), m_timer(new GameTimer(0.0f))
			{}

			void Flee::OnStart() {
				m_timer->ResetTimer(m_paramPtr->time);
				DebugObject::AddString(L"FleeStart");
			}

			bool Flee::OnUpdate() {
				MoveUpdate();
				RotationUpdate();

				return IsEnd();
			}

			void Flee::OnExit() {
				DebugObject::AddString(L"FleeEnd");
			}

			void Flee::MoveUpdate() {
				auto ownerObject = GetOwner()->GetGameObject();
				auto targetManager = ownerObject->GetComponent<TargetManager>(false);
				auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
				if (velocityManager && targetManager && targetManager->HasTarget()) {
					auto velocity = velocityManager->GetVelocity();
					auto toTargetVec = targetManager->CalcuToTargetVec();
					auto force = UtilityVelocity::CalucSeekVec(velocity, -toTargetVec, m_paramPtr->maxSpeed);
					velocityManager->AddForce(force);
				}
			}

			void Flee::RotationUpdate() {
				auto ownerObject = GetOwner()->GetGameObject();
				auto rotationController = ownerObject->GetComponent<RotationController>(false);
				auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
				if (velocityManager && rotationController) {
					rotationController->SetDirect(velocityManager->GetVelocity());
				}
			}


			bool Flee::IsEnd() {
				//時間経過で終了するなら
				if (m_paramPtr->isTimer) {
					m_timer->UpdateTimer();
					return m_timer->IsTimeUp();
				}

				return false; //そうでないなら一生終了しない。
			}
		}
	}
}