/*!
@file   LifeSpan.cpp
@brief  寿命を持たせるためのコンポーネントの実体
*/

#include "stdafx.h"
#include "LifeSpan.h"

namespace basecross {
	void LifeSpan::OnUpdate() {
		if (m_timer.Count()) {
			if (m_isRemove) {
				// このコンポーネントのオーナーを削除
				GetGameObject()->GetStage()->RemoveGameObject<GameObject>(GetGameObject());
			}
			else {
				GetGameObject()->SetActive(false);
			}
		}
	}
}