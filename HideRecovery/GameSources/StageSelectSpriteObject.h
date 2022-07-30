/*!
@file StageSelectSpriteObject.h
@brief StageSelectSpriteObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class SpriteObject;
	class Number;

	//--------------------------------------------------------------------------------------
	/// ステージセレクトスプライトオブジェクト
	//--------------------------------------------------------------------------------------
	class StageSelectSpriteObject : public GameObject
	{
	private:
		ex_weak_ptr<SpriteObject> m_unSelectBack;		//非選択時のバックスプライト
		ex_weak_ptr<SpriteObject> m_selectBack;			//選択時のバックスプライト
		ex_weak_ptr<SpriteObject> m_main;				//メインスプライト
		ex_weak_ptr<SpriteObject> m_stage;				//ステージスプライト
		ex_weak_ptr<Number> m_number;					//ナンバースプライト

		std::vector<ex_weak_ptr<GameObject>> m_objects; //生成した全てのオブジェクト配列

		uint32_t m_num;          //ステージナンバー
		wstring m_stageTexture;  //ステージテクスチャ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		/// <param name="num">ステージナンバー</param>
		/// <param name="stageTexture">ステージテクスチャ</param>
		StageSelectSpriteObject(const std::shared_ptr<Stage>& stage, const uint32_t num, const wstring& stageTexture);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// バックスプライトの生成
		/// </summary>
		void CreateBack();

		/// <summary>
		/// メインスプライトの生成
		/// </summary>
		void CreateMain();

		/// <summary>
		/// ステージスプライトの生成
		/// </summary>
		void CreateStage();

		/// <summary>
		/// ナンバースプライトの生成
		/// </summary>
		void CreateNum();
	};

}

//endbasecross