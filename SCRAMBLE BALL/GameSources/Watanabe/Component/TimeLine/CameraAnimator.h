#pragma once
#include "stdafx.h"

namespace basecross {
	struct CameraKeyFrameData {
		Vec3 Eye;			// カメラ位置
		Vec3 At;			// カメラ注視点
		float Time;			// キーフレーム位置
		Lerp::rate Rate;	// このフレームからの保管方法

		bool operator<(const CameraKeyFrameData& right) const {
			return Time < right.Time;
		}
	};
	class CameraAnimator :public Component {
		vector<CameraKeyFrameData> m_keyFrameList;

		void KeyFrameListSort();
	public:
		CameraAnimator(const shared_ptr<GameObject>& owner);

		void OnCreate()override;
		void OnUpdate()override;
		void OnDraw()override {}
	};
}
