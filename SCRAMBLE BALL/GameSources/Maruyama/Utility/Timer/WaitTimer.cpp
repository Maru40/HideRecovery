
/*!
@file WaitTimer.cpp
@brief WaitTimer�N���X����
�S���F�ێR�T��
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

			if (param->isEnd) {  //�I�����Ă�����continue
				continue;
			}

			//���Ԃ̌v���ƏI������Ď�
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