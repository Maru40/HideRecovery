
/*!
@file AIPlayerStator.cpp
@brief AIPlayerStatorのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "StatorBase.h"

#include "AIPlayerStator.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/StateMachine/EnemyMainStateMachine.h"

#include "Maruyama/Enemy/Component/Stator/StateNode/StateNode_HidePlacePatrol.h"
#include "Maruyama/Enemy/Component/Stator/StateNode/StateNode_Buttle.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Utility/ObserveIsInEyeTarget.h"

#include "Maruyama/Utility/SingletonComponent/GameManager.h"

#include "Maruyama/Interface/I_FactionMember.h"
#include "Maruyama/Interface/I_TeamMember.h"
#include "Maruyama/Enemy/AIDirector/EnemyAIDirector.h"
#include "Maruyama/Enemy/AIDirector/FactionCoordinator.h"
#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/Enemy/AIDirector/CombatCoordinator.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		///	AIPlayerStatorの遷移条件メンバー
		//--------------------------------------------------------------------------------------

		AIPlayerStator_TransitionMember::AIPlayerStator_TransitionMember():
			hidePatrolEeyRange(5.0f),
			buttleStartEyeRange(30.0f)
		{}

		//--------------------------------------------------------------------------------------
		///	遷移条件式
		//--------------------------------------------------------------------------------------

		bool IsGameState(const AIPlayerStator::TransitionMember& member) {
			//ゲーム状態かどうかを判断
			return GameManager::GetInstance()->IsCurrentState(GameManager::State::Game);
		}

		bool AIPlayerStator::IsFindButtleTarget(const TransitionMember& member) {
			auto tupleSpace = m_tupler.lock()->GetTupleSpace();
			auto buttleTransition = tupleSpace->Take<Tuple::ButtleTransition>();

			//メッセージが届いていたら。
			if (buttleTransition && buttleTransition->GetValue() < member.buttleStartEyeRange) {
				//ターゲットの設定
				auto targetManager = m_targetManager.lock();
				auto target = buttleTransition->GetTarget();
				targetManager->SetTarget(target);

				//ファクションの変更を通知
				auto faction = m_factionMember.lock()->GetFactionCoordinator();
				faction->TransitionFaction<CombatCoordinator>(m_factionMember.lock());

				return true;
			}

			return false;
		}

		bool AIPlayerStator::IsLostButtleTarget(const TransitionMember& member) {


			return false;
		}

		//--------------------------------------------------------------------------------------
		///	AIPlayerStator本体
		//--------------------------------------------------------------------------------------

		AIPlayerStator::AIPlayerStator(const std::shared_ptr<GameObject>& objPtr):
			StatorBase(objPtr),
			m_eye(objPtr->GetComponent<EyeSearchRange>())
			//m_observeButtleTarget(new ObserveIsInEyeTarget(m_eye.lock()))
		{
			m_teamMember = objPtr->GetComponent<I_TeamMember>(false);
			m_factionMember = objPtr->GetComponent<I_FactionMember>(false);
			m_tupler = objPtr->GetComponent<Tuple::I_Tupler>(false);
			m_targetManager = objPtr->GetComponent<TargetManager>(false);
		}

		void AIPlayerStator::OnLateStart() {

		}

		void AIPlayerStator::CreateNode() {
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();

			//None状態
			m_stateMachine->AddNode(StateType::None, nullptr);

			//隠し場所を探す状態
			m_stateMachine->AddNode(StateType::HidePlacePatrol, std::make_shared<StateNode::HidePlacePatrol>(enemy));

			//バトル
			m_stateMachine->AddNode(StateType::Buttle, std::make_shared<StateNode::Buttle>(enemy));
		}

		void AIPlayerStator::CreateEdge() {
			auto enemy = GetGameObject()->GetComponent<EnemyBase>();

			//None
			m_stateMachine->AddEdge(StateType::None, StateType::HidePlacePatrol, &IsGameState);
			//m_stateMachine->AddEdge(StateType::None, StateType::Buttle, &IsGameState);

			//隠し場所を探す
			m_stateMachine->AddEdge(
				StateType::HidePlacePatrol, 
				StateType::Buttle, 
				[&](const TransitionMember& member) { return IsFindButtleTarget(member); }
			);

			//バトル
			m_stateMachine->AddEdge(
				StateType::Buttle,
				StateType::HidePlacePatrol,
				[&](const TransitionMember& member) { return IsLostButtleTarget(member); }
			);
		}

	}
}