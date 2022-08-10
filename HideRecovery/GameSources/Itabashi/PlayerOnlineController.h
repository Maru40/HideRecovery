#pragma once
#include "OnlineManager.h"

namespace basecross
{
	class RotationController;

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
		/// <summary>
		/// オブジェクトを動かす用のコンポーネント
		/// </summary>
		std::weak_ptr<Operator::ObjectMover> m_objectMover;
		/// <summary>
		/// オブジェクトの回転制御用コンポーネント
		/// </summary>
		std::weak_ptr<RotationController> m_rotationController;
		/// <summary>
		/// 対応するプレイヤー番号
		/// </summary>
		int m_playerNumber = 0;

	public:
		/// <summary>
		/// オンライン用イベントコード
		/// </summary>
		static constexpr std::uint8_t EVENT_CODE = 2;

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