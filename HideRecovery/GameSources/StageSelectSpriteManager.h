/*!
@file StageSelectSpriteManager.h
@brief StageSelectSpriteManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class SelectSprite;
	class SelecterSprite;

	//--------------------------------------------------------------------------------------
	/// ステージセレクトスプライト管理のパラメータ
	//--------------------------------------------------------------------------------------
	struct StageSelectSpriteManager_Parametor
	{
		float moveRange;  //移動距離
		float moveSpeed;  //移動スピード
		Vec3 moveDirect;  //移動方向

		StageSelectSpriteManager_Parametor();
		StageSelectSpriteManager_Parametor(const float& moveRange, const float& moveSpeed, const Vec3& moveDirect);

	};

	//--------------------------------------------------------------------------------------
	/// ステージセレクトスプライト管理
	//--------------------------------------------------------------------------------------
	class StageSelectSpriteManager : public Component
	{
	public:
		using Parametor = StageSelectSpriteManager_Parametor;

	private:
		Parametor m_param;                                      //パラメータ
		std::vector<ex_weak_ptr<SelectSprite>> m_selectSprites; //選択できるスプライト

		ex_weak_ptr<SelectSprite> m_LBSelect = nullptr;         //LBセレクトスプライト
		ex_weak_ptr<SelectSprite> m_RBSelect = nullptr;         //RBセレクトスプライト

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		StageSelectSpriteManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// ステージセレクト用のスプライトの生成
		/// </summary>
		void CreateStageSelectSprites();

		/// <summary>
		/// RB,LBスプライトの生成
		/// </summary>
		void CreateRBAndLB();

		/// <summary>
		/// セレクタースプライトの生成
		/// </summary>
		void CreateSelecterSprite();

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 移動開始
		/// </summary>
		/// <param name="direct">移動方向</param>
		void StartMove(const Vec3& direct);
	};

}