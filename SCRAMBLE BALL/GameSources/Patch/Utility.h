#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Utility
	{
		Vec3 ConvertWorldToScreen(const shared_ptr<ViewBase>& view, const Vec3& position);
	}
}