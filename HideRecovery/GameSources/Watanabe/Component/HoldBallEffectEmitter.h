﻿#pragma once
#include "stdafx.h"
#include "ItemBag.h"
#include "Watanabe/Effekseer/EfkEffect.h"

namespace basecross {
	class HoldBallEffectEmitter :public Component {
		weak_ptr<ItemBag> m_itemBag;
		weak_ptr<EfkComponent> m_efkComp;
		wstring m_effectKey;
	public:
		HoldBallEffectEmitter(const shared_ptr<GameObject>& owner,
			const shared_ptr<ItemBag>& itemBag);

		void OnCreate()override;
		void OnLateStart()override;
		void OnUpdate()override;
	};
}
