#pragma once
#include "stdafx.h"
#include "UIHelper.h"
#include "../Utility/TransformData.h"

namespace basecross {
	class UIObjectBase :public GameObject {
	protected:
		shared_ptr<RectTransform> rectTransform;
		RectTransformData m_rectTransformData;
		wstring m_name;    //ステージオブジェクトネーム
	public:
		UIObjectBase(const shared_ptr<Stage>& stage, const wstring& name);

		void OnPreCreate() override;
	};
}
