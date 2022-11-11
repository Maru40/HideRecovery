#pragma once
#include "stdafx.h"

namespace basecross {
	template<class T>
	struct KeyFrameData {
		float Time;			// キーフレーム位置
		Lerp::rate Rate;	// このフレームからの保管方法

		/// <summary>
		/// 補間する
		/// </summary>
		virtual void Interpolation(const shared_ptr<T>& keyFrame) = 0;

		bool operator<(const KeyFrameData& right) const {
			return Time < right.Time;
		}
	};

	struct CameraKeyFrameData :public KeyFrameData<CameraKeyFrameData> {
		Vec3 Eye;
		Vec3 At;

		void Interpolation(const shared_ptr<CameraKeyFrameData>& keyFrame)override {
		}
	};
}
