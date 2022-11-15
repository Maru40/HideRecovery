#pragma once
#include "stdafx.h"
#include "ClipBase.h"
#include "CameraKeyFrame.h"

namespace basecross {
	namespace timeline {
		struct CameraData {
			Vec3 Eye;
			Vec3 At;
		};

		class CameraClip :public ClipBase {
			weak_ptr<Camera> m_camera;
		public:
			CameraClip(const shared_ptr<Stage>& stage, const wstring& clipName);

			void Create()override;

			shared_ptr<void> Interpolation(
				const shared_ptr<KeyFrameBase>& current,
				const shared_ptr<KeyFrameBase>& next,
				float delta)override;

			void ApplyDataToObject(const shared_ptr<void>& objectAnimationData)override;
		};
	}
}
