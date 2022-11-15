#pragma once
#include "stdafx.h"
#include "ClipBase.h"

namespace basecross {
	namespace timeline {
		class CameraClip :public ClipBase {
		public:
			CameraClip(const shared_ptr<GameObject>& owner);
		};
	}
}
