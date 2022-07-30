
/*!
@file BillBoard_Ex.h
@brief BillBoard_Exクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	完全なビルボード
	//--------------------------------------------------------------------------------------
	class BillBoard_Ex : public Component
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		BillBoard_Ex(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;
	};

}