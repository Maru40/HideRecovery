/*!
@file MainStage.h
@brief MainStage
*/

#pragma once
#include "stdafx.h"
#include "GameStageBase.h"
#include "Watanabe/UI/GameStartUI.h"
#include "Watanabe/UI/GameFinishUI.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// メインステージ
	//--------------------------------------------------------------------------------------
	class MainStage : public GameStageBase {
		static wstring sm_loadMapName;  //読み込むマップ名

		//std::shared_ptr<GameStartUI> m_gameStartUI;
		//std::shared_ptr<GameFinishUI> m_gameFinishUI;

		/// <summary>
		/// ビューの作成
		/// </summary>
		void CreateViewLight();

	public:

		MainStage() :GameStageBase() {}
		virtual ~MainStage() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 読み込むマップ名の設定
		/// </summary>
		/// <param name="name">読み込むマップ名</param>
		static void SetLoadMapName(const wstring& name);

		/// <summary>
		/// 読み込むマップ名の取得
		/// </summary>
		/// <returns></returns>
		static wstring GetLoadMapName();

		//std::shared_ptr<GameStartUI> GetGameStartUI();
		//std::shared_ptr<GameFinishUI> GetGameFinishUI();
	};
}
//end basecross