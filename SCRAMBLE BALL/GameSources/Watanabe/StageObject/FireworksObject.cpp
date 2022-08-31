#include "stdafx.h"
#include "FireworksObject.h"

namespace basecross {
	FireworksObject::FireworksObject(const shared_ptr<Stage>& stage)
		:StageObjectBase(stage, L"FireworksObject")
	{}

	void FireworksObject::OnCreate() {
		m_efkComp = AddComponent<EfkComponent>();
		m_efkComp.lock()->SetEffectResource(L"Fireworks");
	}

	void FireworksObject::Start() {
		m_efkComp.lock()->Play(L"Fireworks");
	}
	void FireworksObject::Reset() {
		m_efkComp.lock()->Stop(L"Fireworks");
	}
}