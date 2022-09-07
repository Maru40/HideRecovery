
/*!
@file WaitTimer.cpp
@brief WaitTimerクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "WaitTimer.h"

namespace basecross {

	void WaitTimer::TimerUpdate() {
		for (pair<type_index, std::shared_ptr<WaitTimerParametor>> paramPair : m_params) {
			auto& param = paramPair.second;

			auto delta = TimeHelper::GetElapsedTime(param->deltaType);

			if (param->isEnd) {  //終了していたらcontinue
				continue;
			}

			//時間の計測と終了判定監視
			param->timeElapsed += delta * param->countSpeed;
			if (param->timeElapsed >= param->time) {
				param->EndTimer();
			}
		}
	}

	void WaitTimer::OnUpdate() {

		TimerUpdate();
	}

	

}

//endbasecross