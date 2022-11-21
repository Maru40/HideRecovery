#include "stdafx.h"
#include "BallEffectEmitter.h"
#include "../Effekseer/EfkEffect.h"

namespace basecross {
	BallEffectEmitter::BallEffectEmitter(const shared_ptr<GameObject>& owner)
		:Component(owner), m_effectKey(L"HasBall")
	{}

	void BallEffectEmitter::OnLateStart() {
		m_efkComp = GetGameObject()->GetComponent<EfkComponent>();
		m_efkComp.lock()->SetDebugMode(true);
		BallEffectEmitter::OnEnable();
	}

	void BallEffectEmitter::OnEnable() {
		if (auto efkComp = m_efkComp.lock())
			efkComp->PlayLoop(m_effectKey);
	}

	void BallEffectEmitter::OnDisable() {
		if (auto efkComp = m_efkComp.lock())
			efkComp->Stop(m_effectKey);
	}
}