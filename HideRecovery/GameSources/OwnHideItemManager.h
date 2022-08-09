
/*!
@file OwnHideItemManager.h
@brief OwnHideItemManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class HidePlace;

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------
	struct OwnHideItemManager_Parametor {
		float searchHidePlaceRange;			//隠す場所に手が届く範囲
		float putFloorAnimationTime;		//床に置くアニメーション時間
		float putHideObjectAnimationTime;	//隠す場所に置くアニメーション時間

		OwnHideItemManager_Parametor();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="searchHidePlaceRange">隠す場所に手が届く範囲</param>
		OwnHideItemManager_Parametor(const float searchHidePlaceRange);
	};

	//--------------------------------------------------------------------------------------
	/// 持っている隠すアイテムの管理
	//--------------------------------------------------------------------------------------
	class OwnHideItemManager : public Component 
	{
	public:
		using Parametor = OwnHideItemManager_Parametor;

	private:
		Parametor m_param;					//パラメータ
		std::unique_ptr<GameTimer> m_timer;	//時間管理

		bool m_isFleePut;	//デバッグ用のどこでも置けるフラグ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		OwnHideItemManager(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="parametor">パラメータ</param>
		OwnHideItemManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnUpdate() override;

	private:
		/// <summary>
		/// アイテムを置く
		/// </summary>
		void PutHideItem();

		/// <summary>
		/// ブラフ行為
		/// </summary>
		void BluffPutHideItem();

		/// <summary>
		/// アニメーションの再生
		/// </summary>
		/// <param name="putEvent">アニメーションイベント</param>
		void PlayAnimation(const std::function<void()>& putEvent);

		/// <summary>
		/// 回転処理
		/// </summary>
		void Rotation();

		/// <summary>
		/// 置けるかどうかを返す
		/// </summary>
		/// <returns>置けるならtrue</returns>
		bool IsPut() const;

		/// <summary>
		/// 隠す場所の取得
		/// </summary>
		/// <returns>隠す場所を取得</returns>
		Vec3 CalculateHidePosition() const;

		/// <summary>
		/// 隠す場所を探して返す。
		/// </summary>
		/// <returns>隠す場所</returns>
		std::shared_ptr<HidePlace> SearchNearHidePlace() const;
	};

}