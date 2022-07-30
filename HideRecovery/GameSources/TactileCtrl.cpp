/*!
@file TactileCtrl.cpp
@brief TactileCtrlなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "TactileCtrl.h"
#include "MaruUtility.h"

namespace basecross {

	void TactileCtrl::OnCollisionExcute(std::shared_ptr<GameObject>& other) {
		if (other == GetGameObject()->GetParent()) {  //自分の親オブジェクトだったら省く
			return;
		}

		for (auto& action : m_excuteActions) {
			if (action) {
				action(GetThis<TactileCtrl>(), other);
			}
		}
	}
}

//endbasecross