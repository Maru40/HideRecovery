/*!
@file HideItemObject.h
@brief HideItemObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "StageObjectBase.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// 隠すアイテムオブジェクト
	//--------------------------------------------------------------------------------------
	class HideItemObject : public StageObjectBase
	{
	public:
		using DrawComp = PNTStaticDraw;

	private:

	public:
		HideItemObject(const std::shared_ptr<Stage>& stage);
		HideItemObject(const std::shared_ptr<Stage>& stage, const std::wstring& line);

		void OnCreate() override;

	private:
		void SettingModel();
	};
}