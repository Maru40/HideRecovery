#pragma once
#include "stdafx.h"

namespace basecross
{
	/// <summary>
	/// アイテムの機能を持たせるコンポーネント
	/// </summary>
	class Item : public Component
	{
		/// <summary>
		/// アイテム所持者
		/// </summary>
		std::weak_ptr<GameObject> m_itemOwner;

	public:

		Item(const std::shared_ptr<GameObject>& owner);

		void OnUpdate() override {}
		void OnDraw() override {}

		/// <summary>
		/// アイテム所持者の取得
		/// </summary>
		/// <returns>アイテム所持者</returns>
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
	};
}