#pragma once
#include "OnlineManager.h"

namespace basecross
{
namespace Online
{
	/// <summary>
	/// Transform(座標と回転)をオンライン同期するコンポーネント
	/// </summary>
	class OnlineTransformSynchronization : public OnlineComponent
	{
		struct OnlineTransformData
		{
			Vec3 position;
			Quat rotation;

			bool operator==(const OnlineTransformData& other) const;
			bool operator!=(const OnlineTransformData& other) const { return !(*this == other); }
		};

		/// <summary>
		/// Transformコンポーネント
		/// </summary>
		std::weak_ptr<Transform> m_transform;
		/// <summary>
		/// 担当するプレイヤーナンバー
		/// </summary>
		int m_onlinePlayerNumber = OnlineManager::INVALID_ONLINE_PLAYER_NUMBER;

		OnlineTransformData m_beforeData;

		OnlineTransformData GetTransformData() const;

	public:
		/// <summary>
		/// オンラインのイベントコード
		/// </summary>
		static constexpr std::uint8_t EVENT_CODE = 1;

		OnlineTransformSynchronization(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnUpdate() override {}
		void OnUpdate2() override;

		void OnDraw() override {}

		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;

		/// <summary>
		/// 対応するプレイヤー番号の設定
		/// </summary>
		/// <param name="playerNumber">プレイヤー番号</param>
		void SetOnlinePlayerNumber(int playerNumber) { m_onlinePlayerNumber = playerNumber; }
		/// <summary>
		/// 対応するプレイヤー番号の取得
		/// </summary>
		/// <returns>プレイヤー番号</returns>
		int GetOnlinePlayerNumber() const { return m_onlinePlayerNumber; }
	};
}
}