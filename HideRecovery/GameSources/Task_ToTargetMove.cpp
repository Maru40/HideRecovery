/*!
@file Task_ToTargetMove.cpp
@brief Task_ToTargetMoveのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Task_ToTargetMove.h"

#include "UtilityVelocity.h"
#include "VelocityManager.h"

namespace basecross {
	namespace Task {

		//--------------------------------------------------------------------------------------
		///	パラメータ
		//--------------------------------------------------------------------------------------

		Task_ToTargetMove::Parametor::Parametor()
			:Parametor(1.0f, 0.01f, MoveType::Lerp)
		{}

		Task_ToTargetMove::Parametor::Parametor(
			const float& speed, const float& taretNearRange,
			const MoveType moveType, const DeltaType& deltaType
		) :
			Parametor(Vec3(0.0f), Vec3(0.0f), speed, taretNearRange, moveType, deltaType)
		{}

		Task_ToTargetMove::Parametor::Parametor(
			const Vec3& startPosition,
			const Vec3& endPosition,
			const float& speed,
			const float& taretNearRange,
			const MoveType moveType,
			const DeltaType& deltaType
		) :
			startPosition(startPosition),
			endPosition(endPosition),
			speed(speed),
			targetNearRange(taretNearRange),
			moveType(moveType),
			deltaType(deltaType)
		{}

		//--------------------------------------------------------------------------------------
		///	指定箇所まで移動するタスク本体
		//--------------------------------------------------------------------------------------

		Task_ToTargetMove::Task_ToTargetMove(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Parametor>& paramPtr)
			:TaskNodeBase<GameObject>(owner), m_paramPtr(paramPtr)
		{}

		void Task_ToTargetMove::OnStart() {
			GetOwner()->GetComponent<Transform>()->SetPosition(m_paramPtr->startPosition);
		}

		bool Task_ToTargetMove::OnUpdate() {
			if (IsEnd()) {
				return IsEnd();
			}

			MoveUpdate();

			return IsEnd();
		}

		void Task_ToTargetMove::OnExit() {

		}

		void Task_ToTargetMove::SetOwnerPosition(const Vec3& position) {
			GetOwner()->GetComponent<Transform>()->SetPosition(position);
		}

		Vec3 Task_ToTargetMove::GetOwnerPosition() {
			return GetOwner()->GetComponent<Transform>()->GetPosition();
		}

		void Task_ToTargetMove::MoveUpdate() {
			switch (m_paramPtr->moveType)
			{
			case MoveType::Lerp:
				LerpMove();
				break;

			case MoveType::Transform:
				TransformMove();
				break;

			case MoveType::SeekVelocity:
				VelocityMove();
				break;
			case MoveType::ArriveVelocity:
				VelocityMove();
				break;
			}
		}

		void Task_ToTargetMove::LerpMove() {
			auto owner = GetOwner();
			auto delta = GetElapsedTime();
			auto speed = m_paramPtr->speed * delta;

			const float StartTime = 0.0f;
			const float EndTime = 1.0f;
			auto position = Lerp::CalculateLerp(GetOwnerPosition(), m_paramPtr->endPosition, StartTime, EndTime, speed, Lerp::rate::Linear);

			SetOwnerPosition(position);
		}

		void Task_ToTargetMove::TransformMove() {
			auto owner = GetOwner();
			auto ownerTrans = owner->GetComponent<Transform>();
			auto delta = GetElapsedTime();
			auto speed = m_paramPtr->speed * delta;
			auto direction = m_paramPtr->endPosition - GetOwnerPosition();

			auto position = ownerTrans->GetPosition();
			position += direction.GetNormalized() * speed;

			ownerTrans->SetPosition(position);
		}

		void Task_ToTargetMove::VelocityMove() {
			auto owner = GetOwner();
			auto velocityManager = owner->GetComponent<VelocityManager>(false);
			if (!velocityManager) {
				return;
			}

			auto force = CalculateVelocityForce();
			velocityManager->AddForce(force);
		}

		Vec3 Task_ToTargetMove::CalculateVelocityForce() {
			auto owner = GetOwner();
			auto velocityManager = owner->GetComponent<VelocityManager>(false);
			if (!velocityManager) {
				return Vec3(0.0f);
			}

			Vec3 force(0.0f);
			auto toTargetVec = m_paramPtr->endPosition - GetOwnerPosition();

			switch (m_paramPtr->moveType)
			{
			case MoveType::SeekVelocity:
				force = UtilityVelocity::CalucSeekVec(velocityManager->GetVelocity(), toTargetVec, m_paramPtr->speed);
				break;

			case MoveType::ArriveVelocity:
				force = UtilityVelocity::CalucArriveVec(velocityManager->GetVelocity(), toTargetVec, m_paramPtr->speed);
				break;
			}

			return force;
		}

		float Task_ToTargetMove::GetElapsedTime() {
			auto& stepTimer = App::GetApp()->GetStepTimer();

			switch (m_paramPtr->deltaType) {
			case DeltaType::Normal:
				return static_cast<float>(stepTimer.GetElapsedSeconds());
				break;

			case DeltaType::Real:
				return static_cast<float>(stepTimer.GetDefaultElapsedSeconds());
				break;

			default:
				return static_cast<float>(stepTimer.GetElapsedSeconds());
				break;
			};
		}

		bool Task_ToTargetMove::IsEnd() {
			auto toEndPosition = m_paramPtr->endPosition - GetOwnerPosition();

			const float BetweenLength = m_paramPtr->targetNearRange;
			return toEndPosition.length() < BetweenLength ? true : false;
		}

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		void Task_ToTargetMove::SetParametor(const std::shared_ptr<Parametor>& paramPtr) noexcept {
			m_paramPtr = paramPtr;
		}

		std::shared_ptr<Task_ToTargetMove::Parametor> Task_ToTargetMove::GetParametor() const {
			return m_paramPtr.GetShard();
		}

	}
}

//endbaecross