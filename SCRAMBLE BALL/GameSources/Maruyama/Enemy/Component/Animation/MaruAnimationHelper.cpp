
/*!
@file MaruAnimationHelper.cpp
@brief MaruAnimationHelperのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "MaruAnimationHelper.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

#include "TaskList.h"
#include "SeekTarget.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	AnimationTimeEvent
		//--------------------------------------------------------------------------------------

		AnimationTimeEvent::AnimationTimeEvent(const float& time, const std::function<void()> event)
			:time(time), event(event), isEnd(false)
		{}

		//--------------------------------------------------------------------------------------
		///	AnimationClip
		//--------------------------------------------------------------------------------------

		AnimationClip::AnimationClip(const std::shared_ptr<I_Motion>& motion)
			: AnimationClip(motion, false)
		{}

		AnimationClip::AnimationClip(const std::shared_ptr<I_Motion>& motion, const bool isLoop)
			: AnimationClip(motion, isLoop, 1.0f)
		{}

		AnimationClip::AnimationClip(const std::shared_ptr<I_Motion>& motion, const bool isLoop, const float& timeSpeed)
			: m_motion(motion), m_isLoop(isLoop), m_timeSpeed(timeSpeed), m_timer(new GameTimer(0.0f))
		{}

		void AnimationClip::OnStart() {
			for (const auto& node : m_stateNodes) {
				node->OnStart();
			}

			if (m_motion) {
				m_motion->OnStart();
				m_timer->ResetTimer(m_motion->GetTime());
			}
		}

		bool AnimationClip::OnUpdate() {
			if (m_timer->IsTimeUp()) { //時間が終了していたら呼ばない。
				return false;
			}

			m_timer->UpdateTimer(m_timeSpeed);
			MotionUpdate();
			TimeEventManager();

			//追記イベントの呼び出し。
			for (const auto& node : m_stateNodes) {
				node->OnUpdate();
			}

			//時間が終了したら
			if (m_timer->IsTimeUp()) {
				TimerEndFunction();
			}

			return false;
		}

		void AnimationClip::OnExit() {
			if (m_motion) {
				m_motion->OnExit();
			}

			for (const auto& node : m_stateNodes) {
				node->OnExit();
			}
		}

		void AnimationClip::MotionUpdate() {
			if (m_motion) {
				m_motion->OnUpdate(m_timeSpeed);
			}
		}

		void AnimationClip::TimeEventManager() {
			//ゲームタイマーの経過時間割合
			const auto& timerRate = m_timer->GetTimeRate();

			for (auto& timeEvent : m_timeEvents) {
				if (timeEvent.isEnd) {
					continue;
				}

				//経過時間の割合
				const float& eventTimeRate = timeEvent.time / m_timer->GetIntervalTime();
				//経過の割合を過ぎたら
				if (timerRate > eventTimeRate) {
					if (timeEvent.event) { //nullCheck
						timeEvent.event(); //eventの呼び出し。
					}

					timeEvent.isEnd = true; //終了状態にする。
				}
			}
		}

		void AnimationClip::TimerEndFunction() {
			if (IsLoop()) {
				ResetTimeEvents();
				if (m_motion) {
					m_timer->ResetTimer(m_motion->GetTime());
				}
			}
		}

		void AnimationClip::ResetTimeEvents() {
			for (auto& timeEvent : m_timeEvents) {
				//イベントを呼んでいなかったら
				if (!timeEvent.isEnd) {    
					//nullCheck
					if (timeEvent.event) {
						timeEvent.event(); //イベントを呼ぶ
					}
				}

				timeEvent.isEnd = false;
			}
		}

		//--------------------------------------------------------------------------------------
		///	AnimationClipのアクセッサ
		//--------------------------------------------------------------------------------------

		void AnimationClip::AddTimeEvent(const float& time, const std::function<void()>& event) {
			m_timeEvents.push_back(AnimationTimeEvent(time, event));
		}

		void AnimationClip::AddStateNode(const std::shared_ptr<I_StateNode>& stateNode) {
			m_stateNodes.push_back(stateNode);
		}

		bool AnimationClip::IsLoop() const noexcept {
			return m_isLoop;
		}

		bool AnimationClip::IsEnd() const {
			if (IsLoop()) { //ループなら一生終わらない
				return false;
			}

			return m_timer->IsTimeUp();
		}

		//--------------------------------------------------------------------------------------
		///	AnimationStateNodeBase
		//--------------------------------------------------------------------------------------

		AnimationStateNodeBase::AnimationStateNodeBase(const std::shared_ptr<AnimationClip>& animationClip)
			:m_animationClip(animationClip)
		{}

		void AnimationStateNodeBase::OnStart() {
			if (m_animationClip) {
				m_animationClip->OnStart();
			}
		}

		bool AnimationStateNodeBase::OnUpdate() {
			if (m_animationClip) {
				m_animationClip->OnUpdate();
				return m_animationClip->IsEnd();
			}

			return true;
		}

		void AnimationStateNodeBase::OnExit() {
			if (m_animationClip) {
				m_animationClip->OnExit();
			}
		}

		std::shared_ptr<AnimationClip> AnimationStateNodeBase::GetAnimationClip() const noexcept {
			return m_animationClip;
		}

	}

}