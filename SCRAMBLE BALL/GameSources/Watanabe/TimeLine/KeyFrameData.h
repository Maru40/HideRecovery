#pragma once
#include "stdafx.h"
#include "../Utility/TransformData.h"

namespace basecross {
	struct KeyFrameData {
		float Time;			// キーフレーム位置
		Lerp::rate Rate;	// このフレームからの保管方法

		/// <summary>
		/// 補間する
		/// </summary>
		virtual void Interpolation(const shared_ptr<KeyFrameData>& keyFrame) = 0;

		bool operator<(const KeyFrameData& right) const {
			return Time < right.Time;
		}
	};

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
		TransformData m_transformData;
		float Time;			// キーフレーム位置
		Lerp::rate Rate;	// このフレームからの保管方法
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
