
/*!
@file HideItem.h
@brief HideItemなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"
#include "ItemBase.h"

namespace basecross {

	namespace Operator
	{
		class ObjectHider;
	}

	class Item;


	//--------------------------------------------------------------------------------------
	/// 隠すアイテム
	//--------------------------------------------------------------------------------------
	class HideItem : public ItemBase
	{
		std::weak_ptr<Operator::ObjectHider> m_hider;
		std::weak_ptr<Item> m_item;

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		HideItem(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;

	private:

	public:

		std::shared_ptr<Operator::ObjectHider> GetObjectHider() const { return m_hider.lock(); }
		std::shared_ptr<Item> GetItem() const { return m_item.lock(); }
	};

}