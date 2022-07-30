/*!
@file MoveUI.h
@brief MoveUIなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "MoveUI.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// UIの位置を調整したい時に利用するクラス
	//--------------------------------------------------------------------------------------
	class MoveUI : public Component
	{
		float m_speed;  //移動スピード

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		MoveUI(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate();
	};

}