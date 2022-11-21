#pragma once
#include "stdafx.h"
#include "Maruyama/Player/Component/ItemBag.h"
#include "Watanabe/Effekseer/EfkEffect.h"

namespace basecross {
	class BallEffectEmitter :public Component {
		weak_ptr<ItemBag> m_itemBag;
		weak_ptr<EfkComponent> m_efkComp;
		wstring m_effectKey;
	public:
		BallEffectEmitter(const shared_ptr<GameObject>& owner,
			const shared_ptr<ItemBag>& itemBag);

		void OnCreate()override;
		void OnLateStart()override;
		void OnUpdate()override;
	};
}
