/*!
@file TactileCtrl.cpp
@brief TactileCtrl�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "TactileCtrl.h"
#include "Maruyama/Utility/Utility.h"

namespace basecross {

	void TactileCtrl::OnCollisionExcute(std::shared_ptr<GameObject>& other) {
		if (other == GetGameObject()->GetParent()) {  //�����̐e�I�u�W�F�N�g��������Ȃ�
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