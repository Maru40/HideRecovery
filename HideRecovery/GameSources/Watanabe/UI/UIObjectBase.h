#pragma once
#include "stdafx.h"
#include "UIHelper.h"
#include "../Utility/TransformData.h"

namespace basecross {
	class UIObjectBase :public GameObject {
	protected:
		weak_ptr<RectTransform> rectTransform;
		RectTransformData m_rectTransformData;
	public:
		UIObjectBase(const shared_ptr<Stage>& stage);

		void OnPreCreate() override;
	};
}
