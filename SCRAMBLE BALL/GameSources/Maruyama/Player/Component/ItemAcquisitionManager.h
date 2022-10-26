
/*!
@file ItemAcquisitionManager.h
@brief ItemAcquisitionManagerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class Item;
	class SplashMessageUI;

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------
	struct ItemAcquisitionManager_Parametor {
		float searchRange;	//アイテム索敵範囲

		ItemAcquisitionManager_Parametor();

		ItemAcquisitionManager_Parametor(const float searchRange);
	};

	//--------------------------------------------------------------------------------------
	/// アイテム獲得管理
	//--------------------------------------------------------------------------------------
	class ItemAcquisitionManager : public Component
	{
	public:
		using Parametor = ItemAcquisitionManager_Parametor;

	private:
		Parametor m_param;	//パラメータ

		std::vector<std::weak_ptr<Item>> m_allFieldItems;		//フィールドにある全てのアイテムを配列に入れる。
		std::vector<std::weak_ptr<Item>> m_acquisitionItems;	//獲得したアイテム

		std::weak_ptr<SplashMessageUI> m_splashMessageUI;

		std::weak_ptr<SoundEmitter> m_soundEmitter;
		SoundClip m_getBallSoundClip;
		SoundClip m_stolenBallSoundClip;

		bool m_canAcquisition = false;

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト	</param>
		ItemAcquisitionManager(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="param">パラメータ</param>
		ItemAcquisitionManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& param);

		void OnLateStart() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// アイテム獲得のボタンが押された時の処理
		/// </summary>
		void Input_ItemAcquisition();

	public:

		/// <summary>
		/// 隠すアイテムをゲットしたときのイベント
		/// </summary>
		void HideItemAcquisitionEvent(const std::shared_ptr<GameObject>& other);

		/// <summary>
		/// 実際にアイテムを獲得する処理
		/// </summary>
		/// <param name="item">アイテム</param>
		void ItemAcquisition(const std::shared_ptr<Item>& item);

		/// <summary>
		/// 実際にアイテムを獲得する処理
		/// </summary>
		/// <param name="itemId">アイテムID</param>
		void ItemAcquisition(int itemId);


	private:

		/// <summary>
		/// アイテムが獲得範囲内にいるかどうか判断
		/// </summary>
		/// <param name="item">確認したいアイテム</param>
		/// <returns>獲得範囲内ならtrue</returns>
		bool IsAcquisitionRange(const std::shared_ptr<Item>& item);

		/// <summary>
		/// オブジェクトのサーチ範囲用のトリガーを生成する。(多分ボツ)
		/// </summary>
		void CreateSerchTriggerObject();

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// アイテムを取得できるかどうか
		/// </summary>
		/// <param name="item">アイテムが取得できるなら、この変数に入る</param>
		/// <returns>取得できるならtrue</returns>
		bool IsAcquisition(std::shared_ptr<Item>& acquisitionedItem = std::shared_ptr<Item>());

		std::vector<std::shared_ptr<Item>> GetFieldAllItem() const;

		/// <summary>
		/// 現状取得できるアイテムの配列の取得
		/// </summary>
		/// <returns>現状取得できるアイテムの配列</returns>
		std::vector<std::shared_ptr<Item>> GetCanAcquisitionItems() const;
	};

}