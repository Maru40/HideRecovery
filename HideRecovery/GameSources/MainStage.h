/*!
@file MainStage.h
@brief MainStage
*/

#pragma once
#include "stdafx.h"

#include "GameStageBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// メインステージ
	//--------------------------------------------------------------------------------------
	class MainStage : public GameStageBase {
		static wstring sm_loadMapName;  //読み込むマップ名

		/// <summary>
		/// ビューの作成
		/// </summary>
		void CreateViewLight();

	public:

		MainStage() :GameStageBase() {}
		virtual ~MainStage() {}

		virtual void OnCreate() override;

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

	};
}
//end basecross