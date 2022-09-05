/*!
@file PlayerAnimationCtrl.cpp
@brief PlayerAnimationCtrlクラス実体
担当：丸山裕喜
*/


#include "stdafx.h"
#include "Project.h"

#include "AnimationCtrl.h"
#include "PlayerAnimationCtrl.h"

#include "VelocityManager.h"
#include "PlayerInputer.h"

#include "PlayerMover.h"
#include "VelocityManager.h"
#include "PlayerController.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 遷移メンバー
	//--------------------------------------------------------------------------------------

	PlayerAnimationCtrl_TransitionMember::PlayerAnimationCtrl_TransitionMember():
		walkSpeed(2.0f)
	{}

	//--------------------------------------------------------------------------------------
	/// アニメーション追加パラメータ
	//--------------------------------------------------------------------------------------

	AnimationParametor::AnimationParametor() :
		AnimationParametor(State(0), L"", 0, 0, false, 1.0f, nullptr, nullptr, nullptr)
	{}

	AnimationParametor::AnimationParametor(
		const State& state,
		const wstring& name,
		const int startTime,
		const int endTime,
		const bool loop,
		const float updateSpeed,
		const std::shared_ptr<I_AnimationStateNode>& stateNode
	) :
		AnimationParametor(state, name, startTime, endTime, loop, updateSpeed, 
			[stateNode]() { stateNode->OnStart(); },
			[stateNode](const float speed) { return stateNode->OnUpdate(speed); },
			[stateNode]() { stateNode->OnExit(); }
		)
	{}

	AnimationParametor::AnimationParametor(
		const State& state,
		const wstring& name,
		const int startTime,
		const int endTime,
		const bool loop,
		const float updateSpeed,
		const function<void()>& startEvent,
		const function<bool(const float)>& updateEvent,
		const function<void()>& exitEvent
	):
		AnimationParametor(state, name, startTime, endTime, loop, 30.0f, updateSpeed, startEvent, updateEvent, exitEvent)
	{}

	AnimationParametor::AnimationParametor(
			const State& state,
			const wstring& name,
			const int startTime,
			const int endTime,
			const bool loop,
			const float parSecond,
			const float updateSpeed,
			const function<void()>& startEvent,
			const function<bool(const float)>& updateEvent,
			const function<void()>& exitEvent
	) :
		stateType(state),
		stateName(name),
		startTime(startTime),
		loop(loop),
		timeParSecond(parSecond),
		updateSpeed(updateSpeed),
		startEvent(startEvent),
		updateEvent(updateEvent),
		exitEvent(exitEvent)
	{
		timeLength = endTime - startTime;
	}

	//--------------------------------------------------------------------------------------
	/// アニメーション管理本体
	//--------------------------------------------------------------------------------------

	PlayerAnimationCtrl::PlayerAnimationCtrl(const std::shared_ptr<GameObject>& objPtr):
		AnimationCtrl(objPtr), m_currentState(State(0)), m_transitionMember(TransitionMember())
	{}

	void PlayerAnimationCtrl::OnCreate() 
	{
		auto object = GetGameObject();

		//メッシュの生成
		m_drawComponent = GetGameObject()->GetComponent<SmBaseDraw>();
		auto drawComp = object->GetComponent<PNTBoneModelDraw>();

		//アニメーション用のパラメータ
		AnimationParametor params[] = {
			//ステートEnum,				ステート名,				開始時間,	終了時間,	ループ,		更新速度,	イベント
			{State::Idle,				L"Idle",				1,			39,			true,		1.0f ,		nullptr, [&](const float speed) { return Idle(speed); }, nullptr },
			{State::Walk,				L"Dash",				99,			119,		true,		1.0f,		nullptr, [&](const float speed) { return Walk(speed); }, nullptr },
			{State::PutItem_Floor,		L"PutItem_Floor",		125,		149,		true,		1.0f,		std::make_shared<AnimationStateNode::PutItem_Floor>(GetGameObject()) },
			{State::PutItem_HideObject,	L"PutItem_HideObject",	150,		174,		true,		1.0f,		std::make_shared<AnimationStateNode::PutItem_HideObject>(GetGameObject()) },
		};

		//アニメーションの設定
		for (auto& param : params)
		{
			drawComp->AddAnimation(
				param.stateName, 
				param.startTime, 
				param.timeLength, 
				param.loop, 
				param.timeParSecond
			);

			m_parametorMap[param.stateType] = param;
		}

		ChangeForceAnimation(params[0].stateType);  //初期アニメ―ション設定
	}

	void PlayerAnimationCtrl::OnStart() {
		m_drawComponent = GetGameObject()->GetComponent<SmBaseDraw>();
	}

	void PlayerAnimationCtrl::OnUpdate()
	{
		m_parametorMap[m_currentState].updateEvent(GetCurrentUpdateSpeed());

		//デバッグ--------------------------------------------------
		//if (PlayerInputer::IsBDown()) {
		//	ChangeAnimation(State::PutItem_Floor);
		//}
		//if (PlayerInputer::IsYDown()) {
		//	ChangeAnimation(State::PutItem_HideObject);
		//}
	}

	//--------------------------------------------------------------------------------------
	/// アニメーションイベント
	//--------------------------------------------------------------------------------------

	bool PlayerAnimationCtrl::Idle(const float speed)
	{
		auto delta = App::GetApp()->GetElapsedTime();

		m_drawComponent.lock()->UpdateAnimation(delta * speed);

		//遷移条件
		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
			if (m_transitionMember.walkSpeed < velocityManager->GetVelocity().length()) {
				ChangeAnimation(State::Walk);
			}
		}

		return false;
	}

	bool PlayerAnimationCtrl::Walk(const float speed)
	{
		auto velocityMananger = GetGameObject()->GetComponent<VelocityManager>(false);

		float delta = App::GetApp()->GetElapsedTime();
		auto moveVec = velocityMananger->GetVelocity();
		
		m_drawComponent.lock()->UpdateAnimation(delta * GetCurrentUpdateSpeed());

		//遷移条件
		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
			if (velocityManager->GetVelocity().length() < m_transitionMember.walkSpeed) {
				ChangeAnimation(State::Idle);
			}
		}

		return false;
	}

	void PlayerAnimationCtrl::DefaultPlay(const bool isEndTransitionIdle) {
		auto delta = App::GetApp()->GetElapsedTime();
		auto draw = m_drawComponent.lock();

		draw->UpdateAnimation(delta * GetCurrentUpdateSpeed());

		//遷移条件
		if (isEndTransitionIdle && draw->GetCurrentAnimationTime() == 0.0f) {
			ChangeAnimation(State::Idle);
		}
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void PlayerAnimationCtrl::ChangeAnimation(const State& state) {
		if (m_currentState == state) {
			return;
		}

		ChangeForceAnimation(state);
	}

	void PlayerAnimationCtrl::ChangeForceAnimation(const State& state) {
		//終了イベント再生
		if (auto& exitEvent = m_parametorMap[m_currentState].exitEvent) {
			exitEvent();
		}

		m_currentState = state;
		m_drawComponent.lock()->ChangeCurrentAnimation(m_parametorMap[state].stateName);

		//スタートイベント再生
		if (auto& startEvent = m_parametorMap[m_currentState].startEvent) {
			startEvent();
		}
	}


	//--------------------------------------------------------------------------------------
	/// アニメーションステートノード
	//--------------------------------------------------------------------------------------

	namespace AnimationStateNode {

		//--------------------------------------------------------------------------------------
		/// 床にアイテムを置くアニメーション
		//--------------------------------------------------------------------------------------

		PutItem_Floor::PutItem_Floor(const std::shared_ptr<GameObject>& objPtr) :
			AnimationStateNodeBase(objPtr)
		{}

		void PutItem_Floor::OnStart() {
			if (auto mover = GetOwner()->GetComponent<PlayerMover>(false)) {
				mover->SetUpdateActive(false);
			}

			if (auto velocityManager = GetOwner()->GetComponent<VelocityManager>(false)) {
				velocityManager->ResetAll();
			}

			if (auto playerContoller = GetOwner()->GetComponent<PlayerController>(false)) {
				playerContoller->SetUpdateActive(false);
			}
		}

		bool PutItem_Floor::OnUpdate(const float speed) {
			auto isUpdtate = AnimationStateNodeBase::OnUpdate(speed);

			//遷移条件
			if (GetModel()->GetCurrentAnimationTime() == 0.0f) {
				auto animationCtrl = GetOwner()->GetComponent<PlayerAnimationCtrl>(false);
				animationCtrl->ChangeAnimation(PlayerAnimationCtrl::State::Idle);
			}

			return isUpdtate;
		}

		void PutItem_Floor::OnExit() {
			if (auto mover = GetOwner()->GetComponent<PlayerMover>(false)) {
				mover->SetUpdateActive(true);
			}

			if (auto playerContoller = GetOwner()->GetComponent<PlayerController>(false)) {
				playerContoller->SetUpdateActive(true);
			}
		}

		//--------------------------------------------------------------------------------------
		/// 隠すオブジェクトにアイテムを置くアニメーション
		//--------------------------------------------------------------------------------------

		PutItem_HideObject::PutItem_HideObject(const std::shared_ptr<GameObject>& objPtr) :
			AnimationStateNodeBase(objPtr)
		{}

		void PutItem_HideObject::OnStart() {
			if (auto mover = GetOwner()->GetComponent<PlayerMover>(false)) {
				mover->SetUpdateActive(false);
			}

			if (auto velocityManager = GetOwner()->GetComponent<VelocityManager>(false)) {
				velocityManager->ResetAll();
			}

			if (auto playerContoller = GetOwner()->GetComponent<PlayerController>(false)) {
				playerContoller->SetUpdateActive(false);
			}
		}

		bool PutItem_HideObject::OnUpdate(const float speed) {
			auto isUpdtate = AnimationStateNodeBase::OnUpdate(speed);

			//遷移条件
			if (GetModel()->GetCurrentAnimationTime() == 0.0f) {
				auto animationCtrl = GetOwner()->GetComponent<PlayerAnimationCtrl>(false);
				animationCtrl->ChangeAnimation(PlayerAnimationCtrl::State::Idle);
			}

			return isUpdtate;
		}

		void PutItem_HideObject::OnExit() {
			if (auto mover = GetOwner()->GetComponent<PlayerMover>(false)) {
				mover->SetUpdateActive(true);
			}

			if (auto playerContoller = GetOwner()->GetComponent<PlayerController>(false)) {
				playerContoller->SetUpdateActive(true);
			}
		}

	}

}

//endbasecross