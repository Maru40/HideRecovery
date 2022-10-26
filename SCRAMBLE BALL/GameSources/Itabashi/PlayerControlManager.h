#pragma once
#include "stdafx.h"

namespace basecross
{
	class Teleport;
	class VelocityManager;
	class ItemAcquisitionManager;
	class Item;

	/// <summary>
	/// プレイヤーに対しての操作をまとめたクラス
	/// </summary>
	class PlayerControlManager : public Component
	{
		/// <summary>
		/// テレポート機能コンポーネント
		/// </summary>
		std::weak_ptr<Teleport> m_teleport;
		/// <summary>
		///	アイテムの取得管理コンポーネント
		/// </summary>
		std::weak_ptr<ItemAcquisitionManager> m_acquisitionManager;

		std::weak_ptr<VelocityManager> m_velocityManager;

	public:
		PlayerControlManager(const std::shared_ptr<GameObject>& owner) : Component(owner) {}

		void OnLateStart() override;

		void OnUpdate() override {}

		void OnDraw() override {}

		/// <summary>
		/// 取得できるアイテムがあるか試す
		/// </summary>
		/// <param name="findItem">取得できるならそのアイテムが入る</param>
		/// <returns>取得できるならtrue</returns>
		bool TryFindAquisitionableItem(std::shared_ptr<Item>* findItem);

		/// <summary>
		/// アイテムの取得を試す
		/// </summary>
		/// <param name="item">取得するアイテム</param>
		/// <returns>成功したらtrue</returns>
		bool TryAquisition(const std::shared_ptr<Item>& item);
		/// <summary>
		/// アイテムの取得を試す
		/// </summary>
		/// <param name="itemId">取得アイテムID</param>
		/// <returns>成功したらtrue</returns>
		bool TryAquisition(int itemId);

		/// <summary>
		/// マップの開閉を試す
		/// </summary>
		/// <returns>成功したらtrue</returns>
		bool TryOpenMap();

		bool TryTeleport(Vec3* teleportPosition, Vec3* cameraPosition);
		void ExecuteTeleport(const Vec3& temeportPosition, const Vec3& cameraPosition);
	};
}