#pragma once
#include "stdafx.h"
#include "../Utility/TransformData.h"

namespace basecross {
	namespace timeline {
		struct KeyFrameBase {
			float Time;			// キーフレーム位置
			Lerp::rate Rate;	// このフレームからの保管方法

			KeyFrameBase(float time, const Lerp::rate& rate)
				:Time(time), Rate(rate)
			{}

			// ソート用
			bool operator<(const KeyFrameBase& right) const {
				return Time < right.Time;
			}
		};
	}

	struct CameraData {
		Vec3 Eye;
		Vec3 At;
	};

	struct CameraKeyFrameData {
		Vec3 Eye;
		Vec3 At;
		float Time;			// キーフレーム位置
		Lerp::rate Rate;	// このフレームからの保管方法

		CameraKeyFrameData(const Vec3& eye, const Vec3& at, float time, const Lerp::rate& rate)
			:Eye(eye), At(at), Time(time), Rate(rate)
		{}

		/// <summary>
		/// 補間する
		/// </summary>
		CameraData Interpolation(const shared_ptr<CameraKeyFrameData>& keyFrame, float delta) {
			CameraData data = {};
			data.Eye = Lerp::CalculateLerp(Eye, keyFrame->Eye, Time, keyFrame->Time, delta, Rate);
			data.At = Lerp::CalculateLerp(At, keyFrame->At, Time, keyFrame->Time, delta, Rate);
			return data;
		}

		// ソート用
		bool operator<(const CameraKeyFrameData& right) const {
			return Time < right.Time;
		}
	};

	struct GameObjectKeyFrameData {
		TransformData Transform;
		float Time;			// キーフレーム位置
		Lerp::rate Rate;	// このフレームからの保管方法

		GameObjectKeyFrameData(const TransformData& data, float time, const Lerp::rate& rate)
			:Transform(data), Time(time), Rate(rate)
		{}

		/// <summary>
		/// 補間する
		/// </summary>
		TransformData Interpolation(const shared_ptr<GameObjectKeyFrameData>& keyFrame, float delta) {
			TransformData data = {};
			data.Position = Lerp::CalculateLerp(Transform.Position, keyFrame->Transform.Position, Time, keyFrame->Time, delta, Rate);
			data.Scale = Lerp::CalculateLerp(Transform.Scale, keyFrame->Transform.Scale, Time, keyFrame->Time, delta, Rate);
			data.Rotation = Lerp::CalculateLerp(Transform.Rotation, keyFrame->Transform.Rotation, Time, keyFrame->Time, delta, Rate);
			return data;
		}
	};

	struct UIObjectKeyFrameData {
		RectTransformData RectTData;
		float Time;			// キーフレーム位置
		Lerp::rate Rate;	// このフレームからの保管方法

		UIObjectKeyFrameData(const RectTransformData& data, float time, const Lerp::rate& rate)
			:RectTData(data), Time(time), Rate(rate)
		{}

		/// <summary>
		/// 補間する
		/// </summary>
		RectTransformData Interpolation(const shared_ptr<UIObjectKeyFrameData>& keyFrame, float delta) {
			RectTransformData data = {};
			data.Position = Lerp::CalculateLerp(RectTData.Position, keyFrame->RectTData.Position, Time, keyFrame->Time, delta, Rate);
			data.Scale = Lerp::CalculateLerp(RectTData.Scale, keyFrame->RectTData.Scale, Time, keyFrame->Time, delta, Rate);
			data.Rotation = Lerp::CalculateLerp(RectTData.Rotation, keyFrame->RectTData.Rotation, Time, keyFrame->Time, delta, Rate);
			return data;
		}
	};
}
