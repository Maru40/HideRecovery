#include "stdafx.h"
#include "GameObjectClip.h"
#include "GameObjectKeyFrame.h"

namespace basecross {
	namespace timeline {
		GameObjectClip::GameObjectClip()
			:ClipBase()
		{}

		shared_ptr<void> GameObjectClip::Interpolation(
			const shared_ptr<KeyFrameBase>& current,
			const shared_ptr<KeyFrameBase>& next,
			float delta) {
			auto _current = static_pointer_cast<GameObjectKeyFrame>(current);
			auto _next = static_pointer_cast<GameObjectKeyFrame>(next);

			TransformData data = {};
			data.Position = Lerp::CalculateLerp(_current->TransData.Position, _next->TransData.Position, _current->Time, _next->Time, delta, _current->Rate);
			data.Scale = Lerp::CalculateLerp(_current->TransData.Scale, _next->TransData.Scale, _current->Time, _next->Time, delta, _current->Rate);
			data.Rotation = Lerp::CalculateLerp(_current->TransData.Rotation, _next->TransData.Rotation, _current->Time, _next->Time, delta, _current->Rate);

			return make_shared<TransformData>(data);
		}

		void GameObjectClip::ApplyDataToObject(const shared_ptr<void>& objectAnimationData) {
			auto data = static_pointer_cast<TransformData>(objectAnimationData);

			auto rectTrans = m_transComp.lock();
			rectTrans->SetPosition(data->Position);
			rectTrans->SetScale(data->Scale);
			rectTrans->SetRotation(data->Rotation);
		}
	}
}