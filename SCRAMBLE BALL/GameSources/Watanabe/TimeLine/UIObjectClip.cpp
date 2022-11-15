#include "stdafx.h"
#include "UIObjectClip.h"
#include "UIObjectKeyFrame.h"

namespace basecross {
	namespace timeline {
		UIObjectClip::UIObjectClip(const shared_ptr<Stage>& stage, const wstring& clipName)
			:ClipBase(stage, clipName)
		{}

		shared_ptr<void> UIObjectClip::Interpolation(
			const shared_ptr<KeyFrameBase>& current,
			const shared_ptr<KeyFrameBase>& next,
			float delta) {
			auto _current = static_pointer_cast<UIObjectKeyFrame>(current);
			auto _next = static_pointer_cast<UIObjectKeyFrame>(next);

			RectTransformData data = {};
			data.Position = Lerp::CalculateLerp(_current->RectData.Position, _next->RectData.Position, _current->Time, _next->Time, delta, _current->Rate);
			data.Scale = Lerp::CalculateLerp(_current->RectData.Scale, _next->RectData.Scale, _current->Time, _next->Time, delta, _current->Rate);
			data.Rotation = Lerp::CalculateLerp(_current->RectData.Rotation, _next->RectData.Rotation, _current->Time, _next->Time, delta, _current->Rate);

			return make_shared<RectTransformData>(data);
		}

		void UIObjectClip::ApplyDataToObject(const shared_ptr<void>& objectAnimationData) {
			auto data = static_pointer_cast<RectTransformData>(objectAnimationData);

			auto rectTrans = m_rectTrans.lock();
			rectTrans->SetPosition(data->Position);
			rectTrans->SetScale(data->Scale);
			rectTrans->SetRotation(data->Rotation);
		}
	}
}