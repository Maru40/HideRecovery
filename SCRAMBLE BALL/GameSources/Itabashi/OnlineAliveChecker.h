#pragma once
#include "OnlineManager.h"

namespace basecross
{
namespace Online
{
	/// <summary>
	/// 接続が続いているかを観測するコンポーネント
	/// </summary>
	class OnlineAliveChecker : public OnlineComponent
	{
	public:
		static constexpr std::uint8_t CONNECT_ALIVE_CHECK_EVENT_CODE = 171; // 接続確認するオンラインイベントコード

	private:

		struct CheckState
		{
			bool isAlive = false;
			float responseCountTime = 0.0f;

			CheckState() noexcept {}

			CheckState(bool isAlive, float responseCountTime) :
				isAlive(isAlive), responseCountTime(responseCountTime) {}
		};

		float m_aliveCheckTime = 1.0f; // 一定間隔で自分が生存を通知する時間
		float m_aliveResponseTime = 1.0f; // 生きていることの応答待ち最大時間

		float m_aliveCheckCount = 0.0f; // 通知用カウント

		std::unordered_map<int, CheckState> m_playerAliveCheckMap; // プレイヤー接続状況マップ

	public:
		OnlineAliveChecker(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnUpdate() override;

		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;

		/// <summary>
		/// プレイヤーの接続確認が取れているか
		/// </summary>
		/// <param name="onlinePlayerNumber">プレイヤー番号</param>
		/// <returns>接続確認が取れているならtrue</returns>
		bool IsPlayerAlive(int onlinePlayerNumber) const { return m_playerAliveCheckMap.at(onlinePlayerNumber).isAlive; }
		/// <summary>
		/// ローカルプレイヤーの接続確認が取れているか
		/// </summary>
		/// <returns>接続確認が取れているならtrue</returns>
		bool IsLocalPlayerAlive() const { return IsPlayerAlive(OnlineManager::GetLocalPlayer().getNumber()); }

		/// <summary>
		/// 一定間隔で自分が生存を通知する時間を設定
		/// </summary>
		/// <param name="aliveCheckTime">一定間隔で自分が生存を通知する時間</param>
		void SetAliveCheckTime(float aliveCheckTime) { m_aliveCheckTime = aliveCheckTime; }
		/// <summary>
		/// 一定間隔で自分が生存を通知する時間の取得
		/// </summary>
		/// <returns>一定間隔で自分が生存を通知する時間</returns>
		float GetAliveCheckTime() const { return m_aliveCheckTime; }
	};
}
}