/*!
@file   InProcessUI.cpp
@brief  処理中を示すUIの実体
*/

#include "stdafx.h"
#include "InProcessUI.h"

namespace basecross {
	InProcessUI::InProcessUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"InProcessUI")
	{}

	InProcessUI::InProcessUI(const shared_ptr<Stage>& stage, const wstring& line)
		: UIObjectBase(stage, L"InProcessUI")
	{
	}

	void InProcessUI::OnCreate() {
	}
	void InProcessUI::OnUpdate() {
	}
}