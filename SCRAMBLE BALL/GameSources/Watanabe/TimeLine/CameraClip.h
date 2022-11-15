#pragma once
#include "stdafx.h"
#include "ClipBase.h"
#include "CameraKeyFrame.h"

namespace basecross {
	namespace timeline {
		class CameraClip :public ClipBase {
			weak_ptr<Camera> m_camera;
		public:
			CameraClip(const shared_ptr<GameObject>& owner);

			void OnCreate()override;

			shared_ptr<void> Interpolation(
				const shared_ptr<KeyFrameBase>& current,
				const shared_ptr<KeyFrameBase>& next,
				float delta)override;

			void ApplyDataToObject(const shared_ptr<void>& objectAnimationData)override;
		};
	}
}
