/*!
@file Task_ToTargetMove.cpp
@brief Task_ToTargetMoveのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Task_ToTargetMove.h"

#include "Maruyama/Utility/UtilityVelocity.h"
#include "VelocityManager.h"

#include "Maruyama/Enemy/Component/AIVirtualController.h"
#include "Itabashi/OnlinePlayerSynchronizer.h"

namespace basecross {
	namespace Task {

		//--------------------------------------------------------------------------------------
		///	パラメータ
		//--------------------------------------------------------------------------------------

		ToTargetMove_Parametor::ToTargetMove_Parametor() :
			ToTargetMove_Parametor(1.0f, 0.01f, MoveType::Lerp)
		{}

		ToTargetMove_Parametor::ToTargetMove_Parametor(
			const float& speed, const float& taretNearRange,
			const MoveType moveType, const DeltaType& deltaType
		) :
			ToTargetMove_Parametor(Vec3(0.0f), Vec3(0.0f), speed, taretNearRange, moveType, deltaType)
		{}

		ToTargetMove_Parametor::ToTargetMove_Parametor(
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
			deltaType(deltaType),
			isAIVirtualController(true)
		{}

		//--------------------------------------------------------------------------------------
		///	指定箇所まで移動するタスク本体
		//--------------------------------------------------------------------------------------

		ToTargetMove::ToTargetMove(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Parametor>& paramPtr)
			:TaskNodeBase<GameObject>(owner), m_paramPtr(paramPtr)
		{
			m_velocityManager = owner->GetComponent<VelocityManager>(false);
			m_virtualController = GetOwner()->GetComponent<AIVirtualController>(false);
			m_onlineSynchronizer = owner->GetComponent<OnlinePlayerSynchronizer>(false);
		}

		void ToTargetMove::OnStart() {
			GetOwner()->GetComponent<Transform>()->SetPosition(m_paramPtr->startPosition);
			
			if (auto velocityManager = m_velocityManager.lock()) {
				velocityManager->SetMaxSpeed(m_paramPtr->speed);
			}
		}

		bool ToTargetMove::OnUpdate() {
			if (IsEnd()) {
				return IsEnd();
			}

			MoveUpdate();

			return IsEnd();
		}

		void ToTargetMove::OnExit() {

		}

		void ToTargetMove::SetOwnerPosition(const Vec3& position) {
			GetOwner()->GetComponent<Transform>()->SetPosition(position);
		}

		Vec3 ToTargetMove::GetOwnerPosition() {
			return GetOwner()->GetComponent<Transform>()->GetPosition();
		}

		void ToTargetMove::MoveUpdate() {
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

		void ToTargetMove::LerpMove() {
			auto owner = GetOwner();
			auto delta = GetElapsedTime();
			auto speed = m_paramPtr->speed * delta;

			const float StartTime = 0.0f;
			const float EndTime = 1.0f;
			auto position = Lerp::CalculateLerp(GetOwnerPosition(), m_paramPtr->endPosition, StartTime, EndTime, speed, Lerp::rate::Linear);

			SetOwnerPosition(position);
		}

		void ToTargetMove::TransformMove() {
			auto owner = GetOwner();
			auto ownerTrans = owner->GetComponent<Transform>();
			auto delta = GetElapsedTime();
			auto speed = m_paramPtr->speed * delta;
			auto direction = m_paramPtr->endPosition - GetOwnerPosition();

			auto position = ownerTrans->GetPosition();
			position += direction.GetNormalized() * speed;

			ownerTrans->SetPosition(position);
		}

		void ToTargetMove::VelocityMove() {
			auto owner = GetOwner();
			auto velocityManager = owner->GetComponent<VelocityManager>(false);
			if (!velocityManager) {
				return;
			}

			auto force = CalculateVelocityForce();

			//VirtualControllerを使うかどうか
			if (m_paramPtr->isAIVirtualController) {
				AIVirtualControllerMove(force);
			}
			else {
				velocityManager->AddForce(force);
			}
		}

		void ToTargetMove::AIVirtualControllerMove(const Vec3& force) {
			auto owner = GetOwner();
			auto velocityManager = owner->GetComponent<VelocityManager>(false);
			auto virtualController = m_virtualController.lock();
			if (!velocityManager) {
				return;
			}

			auto velocity = velocityManager->GetVelocity();
			auto moveDirection = velocity + force;

			moveDirection /= velocityManager->GetMaxSpeed();	//0 ～ 1の間に変更

			auto input = Vec2(moveDirection.x, moveDirection.z);

			m_virtualController.lock()->SetInputDirection(input);

			m_onlineSynchronizer.lock()->Move(input);
		}

		Vec3 ToTargetMove::CalculateVelocityForce() {
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

		float ToTargetMove::GetElapsedTime() {
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

		bool ToTargetMove::IsEnd() {
			auto toEndPosition = m_paramPtr->endPosition - GetOwnerPosition();

			const float BetweenLength = m_paramPtr->targetNearRange;
			return toEndPosition.length() < BetweenLength ? true : false;
		}

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		void ToTargetMove::SetParametor(const std::shared_ptr<Parametor>& paramPtr) noexcept {
			m_paramPtr = paramPtr;
		}

		std::shared_ptr<ToTargetMove::Parametor> ToTargetMove::GetParametor() const {
			return m_paramPtr.GetShard();
		}

	}
}

//endbaecross