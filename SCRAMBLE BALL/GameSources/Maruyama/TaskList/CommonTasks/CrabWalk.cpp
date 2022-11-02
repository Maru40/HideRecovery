
/*!
@file CrabWalk.cpp
@brief CrabWalkのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "CrabWalk.h"

#include "Maruyama/Utility/Timer/GameTimer.h"

#include "Maruyama/Utility/Random.h"
#include "Maruyama/Utility/Utility.h"

#include "VelocityManager.h"
#include "Maruyama/Utility/UtilityVelocity.h"

namespace basecross {

	namespace TaskListNode {

		//--------------------------------------------------------------------------------------
		///	かに歩きタスクのパラメータ
		//--------------------------------------------------------------------------------------

		CrabWalk_Parametor::CrabWalk_Parametor():
			CrabWalk_Parametor(2.0f)
		{}

		CrabWalk_Parametor::CrabWalk_Parametor(const float speed):
			CrabWalk_Parametor(speed, 1.0f, 1.0f)
		{}

		CrabWalk_Parametor::CrabWalk_Parametor(
			const float speed, 
			const float minMoveTime, 
			const float maxMoveTime
		):
			speed(speed), 
			minMoveTime(minMoveTime), 
			maxMoveTime(maxMoveTime)
		{}

		//--------------------------------------------------------------------------------------
		///	かに歩きタスク
		//--------------------------------------------------------------------------------------

		CrabWalk::CrabWalk(const std::shared_ptr<GameObject>& owner, const Parametor* paramPtr) :
			TaskNodeBase(owner),
			m_paramPtr(paramPtr),
			m_directionType(MoveDirectionType::None),
			m_timer(new GameTimer(0.0f))
		{
			m_transform = owner->GetComponent<Transform>();
			m_velocityManager = owner->GetComponent<VelocityManager>(false);
		}

		void CrabWalk::OnStart() {
			//タイマーリセット
			auto time = maru::MyRandom::Random(m_paramPtr->minMoveTime, m_paramPtr->maxMoveTime);
			m_timer->ResetTimer(time);

			//方向設定
			m_directionType = GetRandomMoveDirectionType();
		}

		bool CrabWalk::OnUpdate() {
			MoveUpdate();

			m_timer->UpdateTimer();

			return IsEnd();
		}

		void CrabWalk::OnExit() {

		}

		void CrabWalk::MoveUpdate() {
			auto velocityManager = m_velocityManager.lock();
			if (!velocityManager) {
				return;
			}

			auto velocity = velocityManager->GetVelocity();
			auto force = UtilityVelocity::CalucSeekVec(velocity, CalculateMoveDirection(), m_paramPtr->speed);

			velocityManager->AddForce(force);
		}

		CrabWalk::MoveDirectionType CrabWalk::GetRandomMoveDirectionType() {
			std::vector<MoveDirectionType> directions = {
				MoveDirectionType::None,
				MoveDirectionType::Right,
				MoveDirectionType::Left,
			};

			return maru::MyRandom::RandomArray(directions);
		}

		Vec3 CrabWalk::CalculateMoveDirection() {
			auto transform = m_transform.lock();
			auto result = Vec3(0.0f);

			switch (m_directionType) 
			{
			case MoveDirectionType::None:
				
				break;

			case MoveDirectionType::Right:
				result =  transform->GetRight();
				break;

			case MoveDirectionType::Left:
				result = -transform->GetRight();
				break;
			}

			return result.GetNormalized();
		}

		bool CrabWalk::IsEnd() const {
			return m_timer->IsTimeUp();
		}

	}
}