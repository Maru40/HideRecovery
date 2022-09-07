/*!
@file GameManagerObject.h
@brief GameManagerObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class SlowTimerManager;

	//--------------------------------------------------------------------------------------
	/// ゲームオブジェクトマネージャー
	//--------------------------------------------------------------------------------------
	class GameManagerObject : public GameObject
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		GameManagerObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

	};

}

//endbasecross