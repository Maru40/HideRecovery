#pragma once
#include "stdafx.h"

namespace basecross {
	class EfkComponent;

	class BallEffectEmitter :public Component {
		weak_ptr<EfkComponent> m_efkComp;
		wstring m_effectKey;
	public:
		BallEffectEmitter(const shared_ptr<GameObject>& owner);

		void OnCreate()override {}
		void OnLateStart()override;
		void OnUpdate()override {}

		void OnEnable()override;
		void OnDisable()override;
	};
}
