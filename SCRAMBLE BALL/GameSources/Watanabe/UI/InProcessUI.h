/*!
@file   InProcessUI.h
@brief  処理中を示すUI
*/

#pragma once
#include "stdafx.h"
#include "UIObjectBase.h"

namespace basecross {
	class InProcessUI :public UIObjectBase {
		// 処理中か
		bool m_isProcessing;
	public:
		InProcessUI(const shared_ptr<Stage>& stage);
		InProcessUI(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
		void OnUpdate()override;
	};
}
