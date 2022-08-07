/*!
@file ItemBag.h
@brief ItemBagなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class ItemBase;
	class HideItem;

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------
	struct ItemBag_Parametor {
		uint32 numMaxOwn;	//持てる最大数

		ItemBag_Parametor();

		ItemBag_Parametor(const uint32 numMaxOwn);
	};

	//--------------------------------------------------------------------------------------
	/// アイテムバッグ
	//--------------------------------------------------------------------------------------
	class ItemBag : public Component
	{
	public:
		using Parametor = ItemBag_Parametor;

	private:
		Parametor m_param;							//パラメータ

		std::vector<ex_weak_ptr<ItemBase>> m_items;	//持っているアイテム一覧

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		ItemBag(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;

	private:

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// アイテムの追加
		/// </summary>
		/// <param name="item">追加したいアイテム</param>
		void AddItem(const std::shared_ptr<ItemBase>& item);

		/// <summary>
		/// アイテムの削除
		/// </summary>
		/// <param name="item">削除したいアイテム</param>
		void RemoveItem(const std::shared_ptr<ItemBase>& item);

		/// <summary>
		/// 引数で渡されたアイテムを取得できるかどうか
		/// </summary>
		/// <param name="item">手に入れたいアイテム</param>
		/// <returns>取得できるならtrue</returns>
		bool IsAcquisition(const std::shared_ptr<ItemBase>& item);

		/// <summary>
		/// 隠すアイテムの取得
		/// </summary>
		/// <returns>隠すアイテム</returns>
		std::shared_ptr<HideItem> GetHideItem() const;
	};

}