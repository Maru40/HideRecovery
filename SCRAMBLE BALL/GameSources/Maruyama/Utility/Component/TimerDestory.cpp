/*!
@file TimerDestroy.cpp
@brief TimerDestroy����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "TimerDestroy.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

namespace basecross {

	TimerDestroy::TimerDestroy(const std::shared_ptr<GameObject>& objPtr, const float& time, const std::function<void()>& endFunction)
		:Component(objPtr), m_timer(new GameTimer(time, endFunction))
	{}

	void TimerDestroy::OnUpdate() {
		m_timer->UpdateTimer();

		if (m_timer->IsTimeUp()) {
			GetStage()->RemoveGameObject<GameObject>(GetGameObject());
		}
	}

	//--------------------------------------------------------------------------------------
	///	�A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	float TimerDestroy::GetTimeRate() const {
		return m_timer->GetTimeRate();
	}

	float TimerDestroy::GetIntervalTimeRate() const {
		return m_timer->GetIntervalTimeRate();
	}

}

//endbasecross