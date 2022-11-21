/*!
@file   PlayerAnimator.cpp
@brief  プレイヤーのアニメーションクラス実体
*/

#include "stdafx.h"
#include "PlayerAnimator.h"

#include "VelocityManager.h"

#include "Patch/PlayerInputer.h"
#include "Maruyama/Player/Component/GoalAnimationController.h"
#include "Maruyama/StageObject/Goal.h"
#include "Maruyama/Utility/Utility.h"

#include "Itabashi/PlayerControlManager.h"
#include "Itabashi/ObjectMover.h"
#include "Itabashi/OnlinePlayerSynchronizer.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// 遷移データ
	//--------------------------------------------------------------------------------------

	TransitionData::TransitionData() :
		TransitionData(nullptr, PlayerAnimationState::State(0))
	{}

	TransitionData::TransitionData(const std::function<bool()>& isTransition, const PlayerAnimationState::State& state) :
		isTransition(isTransition), transitionState(state)
	{}

	//--------------------------------------------------------------------------------------
	/// アニメーションイベント
	//--------------------------------------------------------------------------------------

	AnimationEvent::AnimationEvent() :
		AnimationEvent(nullptr, nullptr, nullptr)
	{}

	AnimationEvent::AnimationEvent(
		const std::function<void()>& start,
		const std::function<bool()>& update,
		const std::function<void()>& exit
	) :
		start(start),
		update(update),
		exit(exit)
	{}

	//--------------------------------------------------------------------------------------
	/// タイムアニメーションイベント
	//--------------------------------------------------------------------------------------

	TimeAnimationEvent::TimeAnimationEvent() :
		TimeAnimationEvent(0.0f, nullptr)
	{}

	TimeAnimationEvent::TimeAnimationEvent(const float time, const std::function<void()>& timeEvent) :
		time(time),
		timeEvent(timeEvent),
		isActive(true)
	{}

	//--------------------------------------------------------------------------------------
	/// プレイヤーアニメーター本体
	//--------------------------------------------------------------------------------------

	PlayerAnimator::PlayerAnimator(const shared_ptr<GameObject>& owner) :
		Animator(owner),
		m_waitTransitionSpeed(0.1f),
		m_walkTransitionSpeed(5.0f),
		m_beforeAnimationTime(0.0f)
	{}

	void PlayerAnimator::OnCreate() {
		auto data = LoadAnimationData(L"PlayerAnimation");
		for (auto d : data) {
			RegisterAnimationClip(d);
		}
	}

	void PlayerAnimator::OnLateStart() {
		m_velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
		m_playerControlManager = GetGameObject()->GetComponent<PlayerControlManager>(false);
		m_objectMover = GetGameObject()->GetComponent<Operator::ObjectMover>(false);
		m_onlineSychronizer = GetGameObject()->GetComponent<OnlinePlayerSynchronizer>(false);

		//床に置くアニメーションのイベント
		AddAnimationEvent(
			PlayerAnimationState::State::PutItem_Floor,
			[&]() { 
				if (m_objectMover.lock()) { m_objectMover.lock()->SetUpdateActive(false); } 
				if (m_playerControlManager.lock()) { m_playerControlManager.lock()->SetUpdateActive(false); }
				if (m_velocityManager.lock()) { m_velocityManager.lock()->ResetAll(); }
				if (m_onlineSychronizer.lock()) { m_onlineSychronizer.lock()->Move(Vec2(0.0f)); }
			},
			nullptr,
			[&]() { 
				if (m_objectMover.lock()) { m_objectMover.lock()->SetUpdateActive(true); } 
				if (m_playerControlManager.lock()) { m_playerControlManager.lock()->SetUpdateActive(true); }
			}
		);

		//自動でwaitに変更したいアニメーション一覧
		PlayerAnimationState::State baseStates[] = {
			PlayerAnimationState::State::PutItem_Floor,
			PlayerAnimationState::State::PutItem_HideObject,
			PlayerAnimationState::State::GunEnd2,
			PlayerAnimationState::State::EndTeleport,
		};

		for (auto& baseState : baseStates) {
			auto state = PlayerAnimationState::PlayerAnimationState2wstring(baseState);
			auto isTransition = [&]() { return IsTargetAnimationEnd(); };
			m_transitionDatasMap[state].push_back(TransitionData(isTransition, PlayerAnimationState::State::Wait));
		}

		{
			// 勝利モーションをLoopさせる
			const size_t IndexSize = 3;
			PlayerAnimationState::State winStates[IndexSize] = {
				PlayerAnimationState::State::Win1,
				PlayerAnimationState::State::Win2,
				PlayerAnimationState::State::Win3,
			};
			// winStatesのループ用
			PlayerAnimationState::State winLoopStates[IndexSize] = {
				PlayerAnimationState::State::Win1Loop,
				PlayerAnimationState::State::Win2Loop,
				PlayerAnimationState::State::Win3Loop,
			};

			for (int i = 0; i < IndexSize; i++) {
				auto strState = PlayerAnimationState::PlayerAnimationState2wstring(winStates[i]);
				auto isTransition = [&]() {return IsTargetAnimationEnd(); };
				// それぞれのモーションに対応したLoopバージョンを遷移先として登録
				m_transitionDatasMap[strState].push_back(TransitionData(isTransition, winLoopStates[i]));
			}
		}

		//自動でshotに遷移したいアニメーション
		{
			auto state = PlayerAnimationState::PlayerAnimationState2wstring(PlayerAnimationState::State::GunSet2);
			auto isTransition = [&]() { return IsTargetAnimationEnd(); };
			m_transitionDatasMap[state].push_back(TransitionData(isTransition, PlayerAnimationState::State::Shot));
		}

		//速度でwaitにするアニメーション
		{
			auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
			wstring states[] = {
				PlayerAnimationState::PlayerAnimationState2wstring(PlayerAnimationState::State::Walk_L),
				PlayerAnimationState::PlayerAnimationState2wstring(PlayerAnimationState::State::Dash),
			};

			;			auto isTransition = [&, velocityManager]() {
				auto velocity = velocityManager->GetVelocity();
				velocity.y = 0;
				return velocity.length() < m_waitTransitionSpeed;
			};

			for (const auto& state : states) {
				m_transitionDatasMap[state].push_back(TransitionData(isTransition, PlayerAnimationState::State::Wait));
			}
		}

		//速度でWalkに変更するスピード
		{
			auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
			wstring states[] = {
				PlayerAnimationState::PlayerAnimationState2wstring(PlayerAnimationState::State::Wait),
			};
			auto isTransition = [&, velocityManager]() {
				auto velocity = velocityManager->GetVelocity();
				velocity.y = 0;
				return velocity.length() > m_waitTransitionSpeed;
			};

			for (const auto& state : states) {
				m_transitionDatasMap[state].push_back(TransitionData(isTransition, PlayerAnimationState::State::Walk_L));
			}

			//Dashの時
			auto state = PlayerAnimationState::PlayerAnimationState2wstring(PlayerAnimationState::State::Dash);

			auto isDashToWalkTransition = [&, velocityManager]() {
				auto velocity = velocityManager->GetVelocity();
				velocity.y = 0;
				return velocity.length() < m_walkTransitionSpeed;
			};

			m_transitionDatasMap[state].push_back(TransitionData(isDashToWalkTransition, PlayerAnimationState::State::Walk_L));
		}

		//速度でDashに変更するスピード
		{
			auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
			wstring states[] = {
				PlayerAnimationState::PlayerAnimationState2wstring(PlayerAnimationState::State::Wait),
				PlayerAnimationState::PlayerAnimationState2wstring(PlayerAnimationState::State::Walk_L),
			};
			auto isTransition = [&, velocityManager]() {
				auto velocity = velocityManager->GetVelocity();
				velocity.y = 0;
				return velocity.length() > m_walkTransitionSpeed;
			};

			for (const auto& state : states) {
				m_transitionDatasMap[state].push_back(TransitionData(isTransition, PlayerAnimationState::State::Dash));
			}
		}

		//Teleportからの遷移
		{
			wstring states[] = {
				PlayerAnimationState::PlayerAnimationState2wstring(PlayerAnimationState::State::StartTeleport),
			};
			auto isTransition = [&]() { return IsTargetAnimationEnd(); };

			for (const auto& state : states) {
				m_transitionDatasMap[state].push_back(TransitionData(isTransition, PlayerAnimationState::State::ExcuteTeleport));
			}
		}
	}

	void PlayerAnimator::OnUpdate2() {
		Animator::OnUpdate();

		Transition();		//遷移
		UpdateEvent();		//更新イベント
		TimeEventUpdate();	//タイムイベント
	}

	void PlayerAnimator::Transition() {
		auto draw = GetGameObject()->GetComponent<ModelDrawComp>();
		auto currentAnimation = draw->GetCurrentAnimation();

		if (m_transitionDatasMap.count(currentAnimation) == 0) {	//メンバーがないなら処理をしない。
			return;
		}

		auto& datas = m_transitionDatasMap.at(currentAnimation);

		for (auto& data : datas) {
			if (!data.isTransition) {
				continue;
			}

			if (data.isTransition()) {
				if ((IsCurretAnimationState(PlayerAnimationState::State::Dead) || IsCurretAnimationState(PlayerAnimationState::State::GSDead)) &&
					data.transitionState != PlayerAnimationState::State::Wait)
				{
					return;
				}

				ChangePlayerAnimation(data.transitionState);
			}
		}
	}

	void PlayerAnimator::StartEvent() {
		auto draw = GetGameObject()->GetComponent<ModelDrawComp>();
		auto currentAnimation = draw->GetCurrentAnimation();
		auto& events = m_animationEventsMap[currentAnimation];
		for (auto& event : events) {
			if (event.start) {
				event.start();
			}
		}
	}

	void PlayerAnimator::UpdateEvent() {
		auto draw = GetGameObject()->GetComponent<ModelDrawComp>();
		auto currentAnimation = draw->GetCurrentAnimation();
		auto& events = m_animationEventsMap[currentAnimation];
		for (auto& event : events) {
			if (event.update) {
				event.update();
			}
		}
	}

	void PlayerAnimator::ExitEvent() {
		auto draw = GetGameObject()->GetComponent<ModelDrawComp>();
		auto currentAnimation = draw->GetCurrentAnimation();
		auto& events = m_animationEventsMap[currentAnimation];
		for (auto& event : events) {
			if (event.exit) {
				event.exit();
			}
		}
	}

	void PlayerAnimator::TimeEventUpdate() {
		auto draw = GetGameObject()->GetComponent<ModelDrawComp>();
		float currentTime = draw->GetCurrentAnimationTime();
		auto currentAnimation = draw->GetCurrentAnimation();
		auto& events = m_timeEventsMap[currentAnimation];
		for (auto& data : events) {
			if (!data.isActive) {	//アクティブでないなら処理をとばす
				continue;
			}

			if (data.time < currentTime) {	//時間を超えていたら。
				if (data.timeEvent) {
					data.timeEvent();
					data.isActive = false;
				}
			}
		}

		//アニメーションがループされたら
		if (currentTime < m_beforeAnimationTime) {
			for (auto& data : events) {
				if (data.isActive) {	//アクティブなら
					if (data.timeEvent) {
						data.timeEvent();
					}
				}

				data.isActive = true;
			}
		}

		m_beforeAnimationTime = currentTime;
	}

	void PlayerAnimator::ChangePlayerAnimation(PlayerAnimationState::State state) {
		if ((IsCurretAnimationState(PlayerAnimationState::State::Dead) || IsCurretAnimationState(PlayerAnimationState::State::GSDead)) &&
			state != PlayerAnimationState::State::Wait)
		{
			return;
		}

		ExitEvent();

		wstring strState = PlayerAnimationState::PlayerAnimationState2wstring(state);
		//タイムイベントのリセット
		m_beforeAnimationTime = 0.0f;
		if (m_timeEventsMap.count(strState) != 0) {
			for (auto& timeEvent : m_timeEventsMap.at(strState)) {
				timeEvent.isActive = true;
			}
		}

		ChangeAnimation(strState);

		StartEvent();
	}

	bool PlayerAnimator::IsCurretAnimationState(const PlayerAnimationState::State& state) const {
		auto drawer = GetGameObject()->GetComponent<ModelDrawComp>();
		auto strState = PlayerAnimationState::PlayerAnimationState2wstring(state);

		return strState == drawer->GetCurrentAnimation();
	}

	void PlayerAnimator::AddAnimationEvent(
		const PlayerAnimationState::State& state,
		const std::function<void()>& start,
		const std::function<bool()>& update,
		const std::function<void()>& exit
	)
	{
		AddAnimationEvent(state, AnimationEvent(start, update, exit));
	}

	void PlayerAnimator::AddAnimationEvent(const PlayerAnimationState::State& state, const AnimationEvent& animationEvent) {
		m_animationEventsMap[PlayerAnimationState::PlayerAnimationState2wstring(state)].push_back(animationEvent);
	}

	void PlayerAnimator::AddTimeEvent(const PlayerAnimationState::State& state, const float time, const std::function<void()>& timeEvent) {
		AddTimeEvent(state, TimeAnimationEvent(time, timeEvent));
	}

	void PlayerAnimator::AddTimeEvent(const PlayerAnimationState::State& state, const TimeAnimationEvent& timeEvent) {
		m_timeEventsMap[PlayerAnimationState::PlayerAnimationState2wstring(state)].push_back(timeEvent);
	}
}