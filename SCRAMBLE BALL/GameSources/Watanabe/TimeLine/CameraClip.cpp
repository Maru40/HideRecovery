#include "stdafx.h"
#include "CameraClip.h"

namespace basecross {
	namespace timeline {
		CameraClip::CameraClip(const shared_ptr<Stage>& stage, const wstring& clipName)
			:ClipBase(stage, clipName)
		{
			Create();
		}

		void CameraClip::Create() {
			m_camera = m_stage.lock()->GetView()->GetTargetCamera();
		}

		shared_ptr<void> CameraClip::Interpolation(
			const shared_ptr<KeyFrameBase>& current,
			const shared_ptr<KeyFrameBase>& next,
			float delta) {
			auto _current = static_pointer_cast<CameraKeyFrame>(current);
			auto _next = static_pointer_cast<CameraKeyFrame>(next);

			CameraData data = {};
			data.Eye = Lerp::CalculateLerp(_current->Eye, _next->Eye, _current->Time, _next->Time, delta, _current->Rate);
			data.At = Lerp::CalculateLerp(_current->At, _next->At, _current->Time, _next->Time, delta, _current->Rate);

			return make_shared<CameraData>(data);
		}

		void CameraClip::ApplyDataToObject(const shared_ptr<void>& objectAnimationData) {
			auto cameraData = static_pointer_cast<CameraData>(objectAnimationData);
			m_camera.lock()->SetEye(cameraData->Eye);
			m_camera.lock()->SetAt(cameraData->At);
		}
	}
}