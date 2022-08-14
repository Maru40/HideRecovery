#pragma once
#include "OnlineManager.h"

namespace basecross
{
	class RotationController;
	class ItemAcquisitionManager;

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

		/// <summary>
		/// 対応するプレイヤー番号
		/// </summary>
		int m_playerNumber = 0;


		void TryAquisition();

		void TryAcquisitionEvent(int itemId, int playerNumber);

		void ExecuteAcquisitionEvent(const ItemOwnerShipData& ownerShipData);

	public:

		PlayerOnlineController(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

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