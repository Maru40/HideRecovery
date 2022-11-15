#pragma once
#include "stdafx.h"
#include "ClipBase.h"
#include "CameraKeyFrame.h"

namespace basecross {
	namespace timeline {
		class UIObjectClip :public ClipBase {
			weak_ptr<RectTransform> m_rectTrans;
		public:
			UIObjectClip();

			void SetTargetRectTransform(const shared_ptr<RectTransform>& obj) { m_rectTrans = obj; }

			shared_ptr<void> Interpolation(
				const shared_ptr<KeyFrameBase>& current,
				const shared_ptr<KeyFrameBase>& next,
				float delta)override;

			void ApplyDataToObject(const shared_ptr<void>& objectAnimationData)override;
		};
	}
}
