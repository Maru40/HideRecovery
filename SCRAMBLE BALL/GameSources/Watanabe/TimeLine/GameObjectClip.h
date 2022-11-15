﻿#pragma once
#include "stdafx.h"
#include "ClipBase.h"

namespace basecross {
	namespace timeline {
		class GameObjectClip :public ClipBase {
			weak_ptr<Transform> m_transComp;
		public:
			GameObjectClip();

			void SetTargetTransform(const shared_ptr<Transform>& obj) { m_transComp = obj; }

			shared_ptr<void> Interpolation(
				const shared_ptr<KeyFrameBase>& current,
				const shared_ptr<KeyFrameBase>& next,
				float delta)override;

			void ApplyDataToObject(const shared_ptr<void>& objectAnimationData)override;
		};
	}
}
