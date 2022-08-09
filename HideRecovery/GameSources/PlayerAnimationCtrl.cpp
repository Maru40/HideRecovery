/*!
@file PlayerAnimationCtrl.cpp
@brief PlayerAnimationCtrlクラス実体
担当：丸山裕喜
*/


#include "stdafx.h"
#include "Project.h"

#include "PlayerAnimationCtrl.h"

#include "VelocityManager.h"
#include "PlayerInputer.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 遷移メンバー
	//--------------------------------------------------------------------------------------

	PlayerAnimationCtrl_TransitionMember::PlayerAnimationCtrl_TransitionMember():
		walkSpeed(2.0f)
	{}

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
		AnimationParametor<PlayerAnimationCtrl> params[] = {
			//ステートEnum,				ステート名,				開始時間,	終了時間,	ループ,		更新速度,	更新イベント
			{State::Idle,				L"Idle",				1,			39,			true,		1.0f ,		&PlayerAnimationCtrl::Idle},
			{State::Walk,				L"Dash",				99,			119,		true,		0.1f,		&PlayerAnimationCtrl::Walk},
			{State::PutItem_Floor,		L"PutItem_Floor",		125,		149,		true,		1.0f,		&PlayerAnimationCtrl::PutItem_Floor},
			{State::PutItem_HideObject,	L"PutItem_HideObject",	150,		174,		true,		1.0f,		&PlayerAnimationCtrl::PutItem_HideObject},
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
		m_parametorMap[m_currentState].updateEvent(*(this));

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

	void PlayerAnimationCtrl::Idle()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		float speed = GetCurrentUpdateSpeed();

		m_drawComponent.lock()->UpdateAnimation(delta * speed);

		//遷移条件
		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
			if (m_transitionMember.walkSpeed < velocityManager->GetVelocity().length()) {
				ChangeAnimation(State::Walk);
			}
		}
	}

	void PlayerAnimationCtrl::Walk()
	{
		auto velocityMananger = GetGameObject()->GetComponent<VelocityManager>(false);

		float delta = App::GetApp()->GetElapsedTime();
		auto moveVec = velocityMananger->GetVelocity();
		
		m_drawComponent.lock()->UpdateAnimation(delta * moveVec.length() * GetCurrentUpdateSpeed());

		//遷移条件
		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
			if (velocityManager->GetVelocity().length() < m_transitionMember.walkSpeed) {
				ChangeAnimation(State::Idle);
			}
		}
	}

	void PlayerAnimationCtrl::PutItem_Floor(){
		DefaultPlay();
	}

	void PlayerAnimationCtrl::PutItem_HideObject() {
		DefaultPlay();
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
			exitEvent(*this);
		}

		m_currentState = state;
		m_drawComponent.lock()->ChangeCurrentAnimation(m_parametorMap[state].stateName);

		//スタートイベント再生
		if (auto& startEvent = m_parametorMap[m_currentState].startEvent) {
			startEvent(*this);
		}
	}

}

//endbasecross