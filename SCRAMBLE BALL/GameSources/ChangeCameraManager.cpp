/*!
@file ChangeCameraManager.cpp
@brief ChangeCameraManagerなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "ChangeCameraManager.h"
#include "PlayerInputer.h"

#include "TimeHelper.h"
#include "GameStageBase.h"
#include "LookTargetManager.h"
#include "MainCameraObject.h"
#include "SeekTPS.h"
#include "SeekTarget.h"
#include "LookAtCameraManager.h"
#include "GameTimer.h"
#include "SlowTimerManager.h"
#include "TackleCameraObject.h"
#include "TargetMarkerManager.h"
#include "HitStopManager.h"

#include "EnemyMainStateMachine.h"

#include "MaruUtility.h"

#include "DebugObject.h"
#include "TargetCameraManager.h"

#include "StateNodeBase.h"
#include "StateNode_Camera_TPS.h"
#include "StateNode_Camera_Look.h"
#include "StateNode_Camera_Tackle.h"

#include "PlayerInputer.h"

#include "SingletonComponent.h"
#include "GameManager.h"

namespace basecross {

	ex_weak_ptr<ChangeCameraManager> maru::SingletonComponent<ChangeCameraManager>::sm_instance = nullptr;

	ChangeCameraManager::ChangeCameraManager(const std::shared_ptr<GameObject>& objPtr)
		:maru::SingletonComponent<ChangeCameraManager>(objPtr), m_tackleTimer(new GameTimer(0.0f)), m_stateMachine(new StateMachine())
	{}

	void ChangeCameraManager::OnLateStart() {
		CreateNode();
		CreateEdge();
	}

	void ChangeCameraManager::OnUpdate2() {
		if (GameManager::GetInstance()->GetCurrentState() == GameManager::State::Clear) {
			return;
		}

		m_stateMachine->OnUpdate();
	}

	void ChangeCameraManager::CreateNode() {
		//TPS
		m_stateMachine->AddNode(State::TPS, std::make_shared<StateNode::Camera_TPS>(GetGameObject()));

		//Look
		m_stateMachine->AddNode(State::Look, std::make_shared<StateNode::Camera_Look>(GetGameObject()));

		//Tackle
		m_stateMachine->AddNode(State::Tackle, std::make_shared<StateNode::Camera_Tackle>(GetGameObject()));
	}

	void ChangeCameraManager::CreateEdge() {
		//Tackle
		m_stateMachine->AddEdge(State::Tackle, State::TPS, [&, this](const TransitionMember& member) { return true; }, true);
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void ChangeCameraManager::ChangeState(const State state) {
		m_stateMachine->ChangeState(state, (int)state);
	}

	ChangeCameraManager::State ChangeCameraManager::GetNowState() const noexcept {
		return m_stateMachine->GetCurrentNodeType();
	}
}

//endbasecross