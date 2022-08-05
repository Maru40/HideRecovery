/*!
@file HideItemObject.h
@brief HideItemObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 隠すアイテムオブジェクト
	//--------------------------------------------------------------------------------------
	class HideItemObject : public GameObject
	{
	public:
		using DrawComp = PNTStaticDraw;

	private:

	public:
		HideItemObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

	private:
		void SettingModel();

	};

}