/*!
@file PlayerAnimationCtrl.cpp
@brief PlayerAnimationCtrlクラス実体
担当：丸山裕喜
*/


#include "stdafx.h"
#include "Project.h"

#include "PlayerAnimationCtrl.h"

#include "VelocityManager.h"

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
		AddAnimeParam<PlayerAnimationCtrl> params[] = {
			{State::Idle,	L"Idle",       1,    39,   true, &PlayerAnimationCtrl::Idle},
			{State::Walk,	L"Dash",       99,  119,   true, &PlayerAnimationCtrl::Walk},

			//{L"Jump",       53,   65,  true, &PlayerAnimationCtrl::Jump},
			//{L"Turn",       66,  105,  true, &PlayerAnimationCtrl::Turn},
			//{L"HundShake", 106,  150,  true, &PlayerAnimationCtrl::HundShake},
			//{L"LookOut",   151,  250,  true, &PlayerAnimationCtrl::LookOut},
			//{L"Appeal",    251,  300,  true, &PlayerAnimationCtrl::Appeal},
			//{L"Fall",      301,  309,  true, &PlayerAnimationCtrl::Fall},
			//{L"Landing",   310,  320,  true, &PlayerAnimationCtrl::Landing},  //着地
			//{L"Dest",      321,  370,  true, &PlayerAnimationCtrl::Dest},
			//{L"TurnFixed",  75,   75,  true, &PlayerAnimationCtrl::TurnFixed},
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

			m_stateStringMap[param.stateType] = param.stateName;	//マップに登録
			SetAnimaiton(param.stateType, param.func);				//アニメーションの登録
		}

		ChangeAnimation(params[0].stateType);  //初期アニメ―ション設定
	}

	void PlayerAnimationCtrl::OnStart() {
		m_drawComponent = GetGameObject()->GetComponent<SmBaseDraw>();
	}

	void PlayerAnimationCtrl::OnUpdate()
	{
		m_animations[m_currentState](*(this));

		//簡易遷移文
		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
			constexpr float DashSpeed = 2.0f;
			if (velocityManager->GetVelocity().length() < DashSpeed) {
				ChangeAnimation(State::Idle);
			}
			else {
				ChangeAnimation(State::Walk);
			}
		}
	}

	void PlayerAnimationCtrl::Idle()
	{
		auto delta = App::GetApp()->GetElapsedTime();
		float speed = 15.0f;

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

		const float speed = 2.0f;
		auto delta = App::GetApp()->GetElapsedTime();

		auto moveVec = velocityMananger->GetVelocity();
		
		auto draw = m_drawComponent.lock();
		draw->UpdateAnimation(delta * moveVec.length() * speed);

		//遷移条件
		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
			if (velocityManager->GetVelocity().length() < m_transitionMember.walkSpeed) {
				ChangeAnimation(State::Idle);
			}
		}
	}

	void PlayerAnimationCtrl::DefaultPlay(const float speed) {
		auto delta = App::GetApp()->GetElapsedTime();
		auto draw = m_drawComponent.lock();

		draw->UpdateAnimation(delta * speed);

		//遷移条件
		if (draw->GetCurrentAnimationTime() == 0.0f) {
			ChangeAnimation(State::Idle);
		}
	}

	void PlayerAnimationCtrl::SetAnimaiton(const State& state, const function<void(PlayerAnimationCtrl&)> func) {
		m_animations[state] = func;
	}

	void PlayerAnimationCtrl::ChangeAnimation(const State& state) {
		if (m_currentState == state) {
			return;
		}

		m_currentState = state;
		m_drawComponent.lock()->ChangeCurrentAnimation(m_stateStringMap[state]);
	}

	void PlayerAnimationCtrl::ChangeForceAnimation(const State& state) {
		m_currentState = state;
		m_drawComponent.lock()->ChangeCurrentAnimation(m_stateStringMap[state]);
	}

}

//endbasecross