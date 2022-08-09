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
		/// <summary>
		/// Transformコンポーネント
		/// </summary>
		std::weak_ptr<Transform> m_transform;
		/// <summary>
		/// 担当するプレイヤーナンバー
		/// </summary>
		int m_playerNumber = 0;
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

		void SetPlayerNumber(int playerNumber) { m_playerNumber = playerNumber; }
		int GetPlayerNumber() const { return m_playerNumber; }
	};
}
}