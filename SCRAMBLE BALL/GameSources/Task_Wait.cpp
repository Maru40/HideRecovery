
/*!
@file Task_Wait.cpp
@brief Task_Waitのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"
#include "TaskList.h"
#include "Task_Wait.h"

namespace basecross {

	namespace Task {

		//--------------------------------------------------------------------------------------
		/// パラメータ
		//--------------------------------------------------------------------------------------

		Wait_Parametor::Wait_Parametor()
			:Wait_Parametor(0.0f)
		{}

		Wait_Parametor::Wait_Parametor(const float& time)
			: Wait_Parametor(time, nullptr, []() { return true; }, nullptr)
		{}

		Wait_Parametor::Wait_Parametor(const float& time, const float& timeUpdateSpeed, const maru::DeltaType& deltaType)
			: Wait_Parametor(time, timeUpdateSpeed, deltaType, nullptr, []() { return true; }, nullptr)
		{}

		Wait_Parametor::Wait_Parametor(const float& time,
			const std::function<void()>& start,
			const std::function<bool()>& update,
			const std::function<void()>& exit
		) :
			Wait_Parametor(time, 1.0f, maru::DeltaType::Normal, start, update, exit)
		{}

		Wait_Parametor::Wait_Parametor(const float& time,
			const float& timeUpdateSpeed,
			const maru::DeltaType& deltaType,
			const std::function<void()>& start,
			const std::function<bool()>& update,
			const std::function<void()>& exit
		):
			time(time), timeUpdateSpeed(timeUpdateSpeed), deltaType(deltaType), start(start), update(update), exit(exit)
		{}

		//--------------------------------------------------------------------------------------
		/// 待機タスク本体
		//--------------------------------------------------------------------------------------

		Wait::Wait(const float& time)
			:Wait(Parametor(time))
		{}

		Wait::Wait(const Parametor& parametor)
			: Wait(std::make_shared<Parametor>(parametor))
		{}

		Wait::Wait(const std::shared_ptr<Parametor>& paramPtr)
			: m_paramPtr(paramPtr), m_timer(new GameTimer(0.0f))
		{}

		void Wait::OnStart() {
			m_timer->ResetTimer(m_paramPtr->time);

			if (m_paramPtr->start) {
				m_paramPtr->start();
			}
		}

		bool Wait::OnUpdate() {
			m_timer->UpdateTimer();

			bool isUpdateEnd = true;
			if (m_paramPtr->update) {
				isUpdateEnd = m_paramPtr->update();
			}

			return m_timer->IsTimeUp() && isUpdateEnd;
		}

		void Wait::OnExit() {
			if (m_paramPtr->exit) {
				m_paramPtr->exit();
			}
		}

	}
}