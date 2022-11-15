
/*!
@file CrabWalk.cpp
@brief CrabWalk�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "CrabWalk.h"

#include "Maruyama/Utility/Timer/GameTimer.h"

#include "Maruyama/Utility/Random.h"
#include "Maruyama/Utility/Utility.h"

#include "VelocityManager.h"
#include "Maruyama/Utility/UtilityVelocity.h"

#include "Itabashi/OnlinePlayerSynchronizer.h"

#include "Itabashi/ObjectMover.h"
#include "Itabashi/PlayerControlManager.h"
#include "Maruyama/Player/Component/UseWeapon.h"

namespace basecross {

	namespace TaskListNode {

		//--------------------------------------------------------------------------------------
		///	���ɕ����^�X�N�̃p�����[�^
		//--------------------------------------------------------------------------------------

		CrabWalk_Parametor::CrabWalk_Parametor():
			//CrabWalk_Parametor(3.0f)
			CrabWalk_Parametor(0.5f)
		{}

		CrabWalk_Parametor::CrabWalk_Parametor(const float speed):
			CrabWalk_Parametor(speed, 0.5f, 1.5f)
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
		///	���ɕ����^�X�N
		//--------------------------------------------------------------------------------------

		CrabWalk::CrabWalk(const std::shared_ptr<GameObject>& owner, const Parametor* paramPtr) :
			TaskNodeBase(owner),
			m_paramPtr(paramPtr),
			m_directionType(MoveDirectionType::None),
			m_timer(new GameTimer(0.0f))
		{
			m_transform = owner->GetComponent<Transform>();
			m_useWepon = owner->GetComponent<UseWeapon>(false);
			m_mover = owner->GetComponent<Operator::ObjectMover>(false);
			m_playerController = owner->GetComponent<PlayerControlManager>(false);
			m_velocityManager = owner->GetComponent<VelocityManager>(false);
			m_onlineSychoronizer = owner->GetComponent<OnlinePlayerSynchronizer>(false);
		}

		void CrabWalk::OnStart() {
			//�^�C�}�[���Z�b�g
			auto time = maru::MyRandom::Random(m_paramPtr->minMoveTime, m_paramPtr->maxMoveTime);
			m_timer->ResetTimer(time);

			//�����ݒ�
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

			auto speed = m_useWepon.lock()->GetWeaponWeight() / m_playerController.lock()->GetDefaultSpeed();
			auto velocity = CalculateMoveDirection() * speed;

			m_onlineSychoronizer.lock()->Move(Vec2(velocity.x, velocity.z));

			//velocityManager->SetVelocity(velocity);

			//auto velocity = velocityManager->GetVelocity();
			//auto force = UtilityVelocity::CalucSeekVec(velocity, CalculateMoveDirection(), m_paramPtr->speed);

			//velocityManager->AddForce(force);
		}

		CrabWalk::MoveDirectionType CrabWalk::GetRandomMoveDirectionType() {
			std::vector<MoveDirectionType> directions = {
				MoveDirectionType::Right,
				MoveDirectionType::Left,
				MoveDirectionType::None,
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
				result = transform->GetRight();
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