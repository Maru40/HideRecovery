
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
	class ItemBase;

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

		std::vector<ex_weak_ptr<ItemBase>> m_allFieldItems;		//フィールドにある全てのアイテムを配列に入れる。

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

		/// <summary>
		/// 実際にアイテムを獲得する処理
		/// </summary>
		void ItemAcquisition(const std::shared_ptr<ItemBase>& item);

		/// <summary>
		/// アイテムが獲得範囲内にいるかどうか判断
		/// </summary>
		/// <param name="item">確認したいアイテム</param>
		/// <returns>獲得範囲内ならtrue</returns>
		bool IsAcquisitionRange(const std::shared_ptr<ItemBase>& item);

		/// <summary>
		/// オブジェクトのサーチ範囲用のトリガーを生成する。(多分ボツ)
		/// </summary>
		void CreateSerchTriggerObject();

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

	};

}