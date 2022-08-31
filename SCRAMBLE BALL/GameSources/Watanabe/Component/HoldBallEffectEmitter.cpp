#include "stdafx.h"
#include "HoldBallEffectEmitter.h"

namespace basecross {
	HoldBallEffectEmitter::HoldBallEffectEmitter(const shared_ptr<GameObject>& owner,
		const shared_ptr<ItemBag>& itemBag)
		:Component(owner), m_itemBag(itemBag), m_effectKey(L"HasBall")
	{}

	void HoldBallEffectEmitter::OnCreate() {
	}

	void HoldBallEffectEmitter::OnLateStart() {
		m_efkComp = GetGameObject()->GetComponent<EfkComponent>();
	}

	void HoldBallEffectEmitter::OnUpdate() {
		auto efkComp = m_efkComp.lock();
		// アイテム持っているとき
		if (m_itemBag.lock()->GetHideItem()) {
			if (!efkComp->IsPlaying(m_effectKey)) {
				efkComp->PlayLoop(m_effectKey);
			}
		}
		else {
			efkComp->Stop(m_effectKey);
		}
	}
}