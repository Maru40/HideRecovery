
/*!
@file Respawner.h
@brief Respawnerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	/// リスポーンをさせるクラス
	//--------------------------------------------------------------------------------------
	class Respawner : public Component
	{

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		Respawner(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
	};

}