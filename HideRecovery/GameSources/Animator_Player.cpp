/*!
@file Animator_Player.cpp
@brief Animator_Playerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Animator_Player.h"

#include "VelocityManager.h"
#include "WeponBase.h"

#include "GameManager.h"

#include "DebugObject.h"

#include "PlayerInputer.h"

#include "TaskList.h"
#include "Task_ToTargetMove.h"
#include "Task_MovePositions.h"

#include "I_Damaged.h"
#include "I_KnockBack.h"
#include "I_Pusher.h"
#include "PlayerStatusManager.h"

#include "SoundManager.h"

namespace basecross {

	PlayerAnimator::PlayerAnimator(const std::shared_ptr<GameObject>& objPtr)
		:Animator(objPtr), m_winMoveParamPtr(nullptr), m_isPlayPlayerWalkSound(false)
	{}

	void PlayerAnimator::OnCreate() {
		Animator::OnCreate();

		CreateNode();
	}

	void PlayerAnimator::OnUpdate() {
		if (GameManager::GetInstance()->GetCurrentState() == GameManager::State::Clear) {
			//return;
		}

		Animator::OnUpdate();

		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
			auto velocity = velocityManager->GetVelocity();
			velocity.y = 0;
			GetMemberRefarence().walkSpeed = velocity.length();
		}

		if ((GetNowState() == StateEnum::Shot ||
			GetNowState() == StateEnum::Aim ||
			GetNowState() == StateEnum::Walk ||
			GetNowState() == StateEnum::WalkStart) &&
			GetMemberRefarence().walkSpeed > GetMemberRefarence().transitionWalkSpeed)
		{
			if (!m_isPlayPlayerWalkSound)
			{
				constexpr float Volume = 0.5f;
				m_isPlayPlayerWalkSound = true;
				SoundManager::GetInstance(GetStage())->PlayOneShot(SoundManager::ID::PlayerWalk, Volume, XAUDIO2_LOOP_INFINITE);
			}
		}
		else
		{
			std::function<void()> func = [&]() {SoundManager::GetInstance(GetStage())->Stop(SoundManager::ID::PlayerWalk); };
		}


	}

	void PlayerAnimator::ChangeState(const StateEnum& stateEnum) {
		auto status = GetGameObject()->GetComponent<PlayerStatusManager>(false);

		if (status && status->IsDeath()) {
			return;
		}

		Animator::ChangeState(stateEnum);
	}

	void PlayerAnimator::ForceChangeState(const StateEnum& stateEnum) {
		Animator::ChangeState(stateEnum);
	}

	void PlayerAnimator::CreateNode() {
		m_stateMachine = CreateAnimationStateMchine();
		
		CreateIdleNode();
		CreateWalkStartNode();
		CreateWalkNode();
		CreateWalkEndNode();
		CreateTackleNode();
		CreateTacleEndNode();
		CreateAimNode();
		CreateShotNode();
		CreateDamageNode();
		CreateDeathNode();
		CreateWinIdleNode();
		CreateWinMoveNode();
	}

	void PlayerAnimator::CreateIdleNode() {
		auto state = m_stateMachine->CreateAnimationState(StateEnum::Idle, L"Player_Idle", 60, true);

		//速度が一定以上なら
		state->AddTransition([](const TransitionMember& member) { return member.walkSpeed > member.transitionWalkSpeed; }, StateEnum::WalkStart, false);
		//AimTriggetOn
		state->AddTransition([](const TransitionMember& member) { return member.aimTrigger.Get(); }, StateEnum::Aim, false);
	}

	void PlayerAnimator::CreateWalkStartNode() {
		auto state = m_stateMachine->CreateAnimationState(StateEnum::WalkStart, L"Player_WalkStart", 9, false);

		state->AddTransition([](const TransitionMember& member) { return true; }, StateEnum::Walk, true);
		//AimTriggetOn
		state->AddTransition([](const TransitionMember& member) { return member.aimTrigger.Get(); }, StateEnum::Aim, false);
	}

	void PlayerAnimator::CreateWalkNode() {
		auto state = m_stateMachine->CreateAnimationState(StateEnum::Walk, L"Player_Walk", 30, true);

		//速度が一定以下なら
		state->AddTransition([](const TransitionMember& member) { return member.walkSpeed <= member.transitionWalkSpeed; }, StateEnum::WalkEnd, false);
		//AimTriggetOn
		state->AddTransition([](const TransitionMember& member) { return member.aimTrigger.Get(); }, StateEnum::Aim, false);

		auto sound = SoundManager::GetInstance(GetStage());
		state->AddExitEvent([&]() {SoundManager::GetInstance(GetStage())->Stop(SoundManager::ID::PlayerWalk); });
		state->AddExitEvent([this]() {m_isPlayPlayerWalkSound = false; });
	}

	void PlayerAnimator::CreateWalkEndNode() {
		auto state = m_stateMachine->CreateAnimationState(StateEnum::WalkEnd, L"Player_WalkEnd", 30, false);

		state->AddTransition([](const TransitionMember& member) { return true; }, StateEnum::Idle, true);
		//AimTriggetOn
		state->AddTransition([](const TransitionMember& member) { return member.aimTrigger.Get(); }, StateEnum::Aim, false);
	}

	void PlayerAnimator::CreateTackleNode() {
		auto state = m_stateMachine->CreateAnimationState(StateEnum::Tackle, L"Player_Tackle", 30, true);

		auto sound = SoundManager::GetInstance(GetStage());
		state->AddEntryEvent([&]() {SoundManager::GetInstance(GetStage())->PlayOneShot(SoundManager::ID::PlayerTackle); });
		state->AddEntryEvent([&]() {SoundManager::GetInstance(GetStage())->Stop(SoundManager::ID::PlayerWalk); });
		state->AddExitEvent([&]() {SoundManager::GetInstance(GetStage())->Stop(SoundManager::ID::PlayerTackle); });
		state->AddExitEvent([this]() {m_isPlayPlayerWalkSound = false; });
	}

	void PlayerAnimator::CreateTacleEndNode() {
		auto state = m_stateMachine->CreateAnimationState(StateEnum::TackleEnd, L"Player_TackleEnd", 30, false);

		state->AddTransition([](const TransitionMember& member) { return true; }, StateEnum::Idle, true);

		state->AddExitEvent([&]() { DebugObject::AddValue(GetGameObject()->GetComponent<VelocityManager>()->GetVelocity().length()); });
	}

	void PlayerAnimator::CreateAimNode() {
		auto state = m_stateMachine->CreateAnimationState(StateEnum::Aim, L"Player_Aim", 9, false);

		state->AddTransition([](const TransitionMember& member) { return true; }, StateEnum::Shot, true);

		auto sound = SoundManager::GetInstance(GetStage());
		//state->AddEntryEvent([&]() { SoundManager::GetInstance(GetStage()) ? SoundManager::GetInstance(GetStage())->PlayOneShot(SoundManager::ID::JettPomp) : nullptr; });
	}

	void PlayerAnimator::CreateShotNode() {
		auto state = m_stateMachine->CreateAnimationState(StateEnum::Shot, L"Player_Shot", 21, true);

		state->AddUpdateEvent([&]() {		
			auto wepon = GetGameObject()->GetComponent<WeponBase>(false);
			if (wepon) {
				wepon->Shot();
			}
		});

		state->AddExitEvent([&]() {
			auto wepon = GetGameObject()->GetComponent<WeponBase>(false);
			if (wepon) {
				wepon->ShotEnd();
			}
		});
		//state->AddAnimationEvent();

		state->AddExitEvent([&]() {SoundManager::GetInstance(GetStage())->Stop(SoundManager::ID::JettPomp); });
		state->AddExitEvent([&]() {SoundManager::GetInstance(GetStage())->Stop(SoundManager::ID::PlayerWalk); });
		state->AddExitEvent([this]() {m_isPlayPlayerWalkSound = false; });
	}

	void PlayerAnimator::CreateDamageNode() {
		auto state = m_stateMachine->CreateAnimationState(StateEnum::Damage, L"Player_Damage", 30, false);

		state->AddTransition([](const TransitionMember& member) { return true; }, StateEnum::Idle, true);
	}

	void PlayerAnimator::CreateDeathNode() {
		constexpr int AnimationTime = 4;
		auto state = m_stateMachine->CreateAnimationState(StateEnum::Death, L"Player_Death", 30 * AnimationTime, false);

		state->AddAnimationEvent(AnimationTime, &PlayerAnimator::EndDeathAnimationEvent, GetThis<PlayerAnimator>());
	}

	void PlayerAnimator::CreateWinIdleNode() {
		constexpr int AnimationTime = 4;
		auto state = m_stateMachine->CreateAnimationState(StateEnum::WinIdle, L"Player_WinIdle", 30 * AnimationTime, false);
	}

	void PlayerAnimator::CreateWinMoveNode() {
		constexpr int AnimationTime = 4;
		auto state = m_stateMachine->CreateAnimationState(StateEnum::WinMove, L"Player_WinMove", 30 * AnimationTime, false);

		enum class WimMoveTaskEnum {
			Move,
		};
		std::shared_ptr<TaskList<WimMoveTaskEnum>> taskList(new TaskList<WimMoveTaskEnum>());

		std::vector<Vec3> positions = {
			Vec3(+50.0f, 10.0f, +40.0f),
			Vec3(-90.0f, 0.0f, +20.0f),
			Vec3(-70.0f, -5.0f, +30.0f),
			Vec3(+30.0f, +5.0f, -25.0f),
			Vec3(-12.5f, -3.0f, +10.0f),
		};
		
		auto param = Task_MovePositions_Parametor();
		param.moveParamPtr->speed = 2.5f;
		param.moveParamPtr->targetNearRange = 7.5f;
		param.moveParamPtr->moveType = Task_ToTargetMove::MoveType::Lerp;
		param.positions = positions;
		param.isSetInitializePosition = true;
		auto paramPtr = std::make_shared<Task_MovePositions_Parametor>(param);
		taskList->DefineTask(WimMoveTaskEnum::Move, std::make_shared<Task_MovePositions>(GetGameObject(), paramPtr));
		taskList->AddTask(WimMoveTaskEnum::Move);

		state->AddUpdateEvent([&, taskList]() { taskList->UpdateTask(); });
		m_winMoveParamPtr = paramPtr;

		state->AddTransition([](const TransitionMember& member) { return true; }, StateEnum::WinIdle, true);
	}

	void PlayerAnimator::EndDeathAnimationEvent() {
		GameManager::GetInstance()->ChangeState(GameManager::State::GameOver);
	}

}