#pragma once
#include "stdafx.h"
#include "UIHelper.h"
#include "../Utility/TransformData.h"

namespace basecross {
	class UIObjectBase :public GameObject {
	protected:
		shared_ptr<RectTransform> rectTransform;
		RectTransformData m_rectTransformData;
		// UI識別用の名前
		wstring m_uiName;
	public:
		UIObjectBase(const shared_ptr<Stage>& stage, const wstring& name);

		void OnPreCreate() override;

		/// <summary>
		/// オブジェクト名の取得
		/// </summary>
		/// <returns>オブジェクト名</returns>
		wstring GetUIName() const { return m_uiName; }
	};
}
