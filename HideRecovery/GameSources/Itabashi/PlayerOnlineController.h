#pragma once
#include "OnlineManager.h"

namespace basecross
{
	class RotationController;
	class ItemAcquisitionManager;
	class OwnHideItemManager;
	class VelocityManager;

namespace Operator
{
	class ObjectMover;
}

namespace Online
{
	/// <summary>
	/// オンラインでのプレイヤーのイベント制御
	/// </summary>
	class PlayerOnlineController : public OnlineComponent
	{
	public:
		struct ItemOwnerShipData
		{
			int itemId;
			int playerNumber;

			ItemOwnerShipData(int itemId, int playerNumber) :
				itemId(itemId),
				playerNumber(playerNumber)
			{

			}
		};

		static constexpr std::uint8_t TRY_ACQUISITION_EVENT_CODE = 2;
		static constexpr std::uint8_t EXECUTE_ACQUISITION_EVENT_CODE = 3;
		static constexpr std::uint8_t TRY_ITEM_HIDE_EVENT_CODE = 4;
		static constexpr std::uint8_t EXECUTE_ITEM_HIDE_EVENT_CODE = 5;
		static constexpr std::uint8_t EXECUTE_MOVE_EVENT_CODE = 6;

	private:

		/// <summary>
		/// オブジェクトを動かす用のコンポーネント
		/// </summary>
		std::weak_ptr<Operator::ObjectMover> m_objectMover;
		/// <summary>
		/// オブジェクトの回転制御用コンポーネント
		/// </summary>
		std::weak_ptr<RotationController> m_rotationController;

		std::weak_ptr<ItemAcquisitionManager> m_acquisitionManager;

		std::weak_ptr<OwnHideItemManager> m_hideItemManager;

		std::weak_ptr<VelocityManager> m_velocityManager;

		/// <summary>
		/// 対応するプレイヤー番号
		/// </summary>
		int m_playerNumber = 0;

		Vec3 m_beforeMoveVector = Vec3();

		std::vector<std::shared_ptr<PlayerOnlineController>> GetPlayerOnlineControllers() const;

		void Move();

		void ExecuteMove(int playerNumber, const Vec3& moveVector);

		void TryAquisition();

		void TryAcquisitionEvent(int itemId, int playerNumber);

		void ExecuteAcquisitionEvent(const ItemOwnerShipData& ownerShipData);

		void TryItemHide();

		void TryItemHideEvent(int playerNumber);

		void ExecuteItemHideEvent(int playerNumber, const Vec3& position);

	public:

		PlayerOnlineController(const std::shared_ptr<GameObject>& owner);

		void OnLateStart() override;

		void OnUpdate() override;

		void OnDraw() override {}

		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;

		/// <summary>
		/// 対応するプレイヤー番号の設定
		/// </summary>
		/// <param name="playerNumber">プレイヤー番号</param>
		void SetPlayerNumber(int playerNumber) { m_playerNumber = playerNumber; }
		/// <summary>
		/// 対応するプレイヤー番号の取得
		/// </summary>
		/// <returns>プレイヤー番号</returns>
		int GetPlayerNumber() const { return m_playerNumber; }
	};
}
}