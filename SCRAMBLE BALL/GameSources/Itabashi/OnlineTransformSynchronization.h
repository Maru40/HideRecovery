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
			int playerNumber;
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

		bool m_isMaster = false; // 同期元であるか

		bool m_isSynchronizePosition = true; // 位置を同期するか
		bool m_isSynchronizeRotation = true; // 回転を同期するか

		float m_synchronizeSpan = 1.0f; // 同期するタイミング
		float m_countSpan = 0.0f; // 同期用の経過時間

		mutable OnlineTransformData m_beforeData;

		std::vector<std::uint8_t> CreateTransformDeltaCompressionData() const;

		void UpdateTransformData(const std::uint8_t* bytes);

	public:
		/// <summary>
		/// オンラインのイベントコード
		/// </summary>
		static constexpr std::uint8_t SYNCHRONIZE_TRANSFORM_ONLINE_EVENT_CODE = 1;

		OnlineTransformSynchronization(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

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

		void SetIsSynchronizePosition(bool isSynchronizePosition) { m_isSynchronizePosition = isSynchronizePosition; }
		bool IsSynchronizePosition() const { return m_isSynchronizePosition; }

		void SetIsSynchronizeRotation(bool isSynchronizeRotation) { m_isSynchronizeRotation = isSynchronizeRotation; }
		bool IsSynchronizeRotation() const { return m_isSynchronizeRotation; }

		void SetIsMaster(bool isMaster) { m_isMaster = isMaster; }
		bool IsMaster() const { return m_isMaster; }

		void SetCountSpan(float countSpan) { m_countSpan = countSpan; }
	};
}
}