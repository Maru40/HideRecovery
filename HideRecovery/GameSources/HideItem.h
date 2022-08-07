
/*!
@file HideItem.h
@brief HideItemなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "ItemBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 隠すアイテム
	//--------------------------------------------------------------------------------------
	class HideItem : public ItemBase
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		HideItem(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;

	private:


	};

}