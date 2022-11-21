#include "stdafx.h"
#include "BallEffectEmitter.h"

namespace basecross {
	BallEffectEmitter::BallEffectEmitter(const shared_ptr<GameObject>& owner,
		const shared_ptr<ItemBag>& itemBag)
		:Component(owner), m_itemBag(itemBag), m_effectKey(L"HasBall")
	{}

	void BallEffectEmitter::OnCreate() {
	}

	void BallEffectEmitter::OnLateStart() {
		m_efkComp = GetGameObject()->GetComponent<EfkComponent>();
	}

	void BallEffectEmitter::OnUpdate() {
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