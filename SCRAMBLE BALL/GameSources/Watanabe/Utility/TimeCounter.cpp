/*!
@file TimeCounter.cpp
@brief 時間計測クラス実体
*/

#include "stdafx.h"
#include "TimeCounter.h"

namespace basecross {
	bool TimeCounter::Count() {
		auto elapsed = App::GetApp()->GetElapsedTime();
		if (m_elaspedTime >= m_intervalTime) {
			m_isTimeUp = true;
		}
		else {
			m_isTimeUp = false;
		}

		// 念のため指定した時間が経過したら停止させる
		if (!m_isTimeUp) {
			m_elaspedTime += elapsed;
		}
		else {
			m_elaspedTime = m_intervalTime;
		}
		return m_isTimeUp;
	}
}
//end basecross