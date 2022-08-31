#pragma once
#include "stdafx.h"

namespace basecross
{
	class Item : public Component
	{
		int m_itemId = 0;
		std::weak_ptr<GameObject> m_itemOwner;

	public:

		Item(const std::shared_ptr<GameObject>& owner);

		void OnUpdate() override {}
		void OnDraw() override {}

		std::shared_ptr<GameObject> GetItemOwner() const { return m_itemOwner.lock(); }

		/// <summary>
		/// アイテムのオーナーを設定する
		/// </summary>
		/// <param name="itemOwner">設定するオーナー</param>
		/// <param name="setIsActive">オーナーをこのオブジェクトの親に設定するか</param>
		void SetItemOwner(const std::shared_ptr<GameObject>& itemOwner, bool isOwnerParent = true);

		/// <summary>
		/// アイテムのオーナーを解放する
		/// </summary>
		/// <param name="isParentClear">親オブジェクトから外すか</param>
		void ReleaseItemOwner(bool isParentClear = true);

		void SetItemId(int itemId) { m_itemId = itemId; }
		int GetItemId() const { return m_itemId; }

		static std::shared_ptr<Item> StageFindToItemId(const std::shared_ptr<Stage>& stage, int itemId);
	};
}