/*!
@file   LifeSpan.cpp
@brief  �������������邽�߂̃R���|�[�l���g�̎���
*/

#include "stdafx.h"
#include "LifeSpan.h"

namespace basecross {
	void LifeSpan::OnUpdate() {
		if (m_timer.Count()) {
			if (m_isRemove) {
				// ���̃R���|�[�l���g�̃I�[�i�[���폜
				GetGameObject()->GetStage()->RemoveGameObject<GameObject>(GetGameObject());
			}
			else {
				GetGameObject()->SetActive(false);
			}
		}
	}
}