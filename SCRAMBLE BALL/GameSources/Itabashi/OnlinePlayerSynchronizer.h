#pragma once
#include "OnlineManager.h"

namespace basecross
{
	class PlayerControlManager;
	class PlayerStatus;
	struct DamageData;

	class OnlinePlayerSynchronizer : public Online::OnlineComponent
	{
	public:

		static constexpr std::uint8_t EXECUTE_CAMERA_FORWARD_EVENT_CODE = 6; // プレイヤー処理に使用するカメラの前方ベクトルの変更を通知するオンラインイベントコード
		static constexpr std::uint8_t EXECUTE_MOVE_EVENT_CODE = 7; // 移動を通知するオンラインイベントコード
		static constexpr std::uint8_t EXECUTE_SHOT_EVENT_CODE = 8; // 弾を撃ったことを通知するオンラインイベントコード
		static constexpr std::uint8_t EXECUTE_BULLET_DESTROY_EVENT_CODE = 9; // 弾が破棄されたことを通知するオンラインイベントコード
		static constexpr std::uint8_t EXECUTE_AIM_STATE_CHANGE_EVENT_CODE = 10; // エイムの変更を通知するオンラインイベントコード
		static constexpr std::uint8_t EXECUTE_TELEPORT_EVENT_CODE = 11; // テレポートしたことを通知するオンラインイベントコード
		static constexpr std::uint8_t TRY_OPEN_HIDEPLACE_EVENT_CODE = 12; // 箱を開けるのを試すことを通知するオンラインイベントコード
		static constexpr std::uint8_t EXECUTE_OPEN_HIDEPLACE_EVENT_CODE = 13; // 箱を開けたことを通知するオンラインイベントコード
		static constexpr std::uint8_t EXECUTE_DAMAGE_EVENT_CODE = 14; // ダメージを受けたことを通知するオンラインイベントコード

		static constexpr int INVALID_GAME_PLAYER_NUMBER = -1;

	private:

		static std::vector<std::weak_ptr<OnlinePlayerSynchronizer>> m_onlinePlayerSynchronizers; // ステージに存在するすべてのOnlinePlayerSynchronizer

		std::weak_ptr<PlayerControlManager> m_controlManager;
		std::weak_ptr<PlayerStatus> m_playerStatus;

		int m_onlinePlayerNumber = Online::OnlineManager::INVALID_ONLINE_PLAYER_NUMBER;	// 対応するオンラインプレイヤー番号
		int m_gamePlayerNumber = INVALID_GAME_PLAYER_NUMBER; // 対応するゲームプレイヤー番号

		Vec3 m_beforeMoveVector = Vec3(); // 1フレーム前の移動量

		std::unordered_map<std::uint32_t, std::weak_ptr<GameObject>> m_bulletObjectMap; // 撃った弾のオブジェクトマップ

		bool m_isBeforeAim = false;


		/// <summary>
		/// カメラの前方ベクトルの更新を確認する
		/// </summary>
		void CheckUpdateCameraForward();
		/// <summary>
		/// 誰かがカメラの前方ベクトルを変更したときに呼ばれるイベント
		/// </summary>
		/// <param name="playerNumber">変更したプレイヤー番号</param>
		/// <param name="cameraForward">カメラの前方ベクトル</param>
		void ExecuteCameraForward(int playerNumber, const Vec3& cameraForward);

		/// <summary>
		/// 誰かが移動したときに呼ばれるイベント
		/// </summary>
		/// <param name="playerNumber">移動したプレイヤー番号</param>
		/// <param name="moveVector">移動量ベクトル</param>
		/// <param name="forward">前方ベクトル</param>
		void ExecuteMove(int playerNumber, const Vec3& moveVector, const Vec3& forward);

		/// <summary>
		/// 誰かが弾を撃った際に呼ばれるイベント
		/// </summary>
		/// <param name="playerNumber">撃ったプレイヤー番号</param>
		/// <param name="bulletPosition">発射位置</param>
		/// <param name="bulletDirection">発射方向</param>
		/// <param name="instanceId">弾のID</param>
		/// <param name="destroyedGameObject">削除する弾のオブジェクト</param>
		void ExecuteShot(int playerNumber, const Vec3& bulletPosition, const Vec3& bulletDirection, std::uint32_t instanceId);

		/// <summary>
		/// 弾を破棄したときに呼ばれるイベント
		/// </summary>
		/// <param name="destroyedGameObject">破棄されるGameObject</param>
		void BulletDestroyed(const std::shared_ptr<GameObject>& destroyedGameObject);
		/// <summary>
		/// 弾が削除されたときに呼ばれるイベント
		/// </summary>
		/// <param name="bulletInstanceId">削除する弾のID</param>
		/// <param name="position">弾の位置</param>
		void ExecuteBulletDestroyEvent(std::uint32_t bulletInstanceId, const Vec3& position);

		/// <summary>
		/// 誰かがエイムを変更したときに呼ばれるイベント
		/// </summary>
		/// <param name="playerNumber">変更したプレイヤー番号</param>
		/// <param name="isAim">エイム状態ならtrue</param>
		void ExecuteAimEvent(int playerNumber, bool isAim);

		/// <summary>
		/// 誰かがテレポートしたら呼ばれるイベント
		/// </summary>
		/// <param name="playerNumber">テレポートしたプレイヤー番号</param>
		/// <param name="teleportPosition">テレポート位置</param>
		/// <param name="cameraPosition">カメラ位置</param>
		void ExecuteTeleportEvent(int playerNumber, const Vec3& teleportPosition, const Vec3& cameraPosition);

		/// <summary>
		/// 誰かが箱を開けるのを試す時に呼ばれるイベント
		/// </summary>
		/// <param name="playerNumber">試すプレイヤー番号</param>
		/// <param name="instanceId">箱のID</param>
		void TryOpenHidePlaceEvent(int playerNumber, std::uint32_t instanceId);
		/// <summary>
		/// 誰かが箱を開けたときに呼ばれるイベント
		/// </summary>
		/// <param name="playerNumber">開けたプレイヤー番号</param>
		/// <param name="instanceId">箱のID</param>
		void ExecuteOpenHidePlace(int playerNumber, std::uint32_t instanceId);

		/// <summary>
		/// ダメージを受けた
		/// </summary>
		/// <param name="playerStatus">プレイヤーのステータス</param>
		/// <param name="damageData">ダメージ情報</param>
		void Damaged(const std::shared_ptr<PlayerStatus>& playerStatus, const DamageData& damageData);
		/// <summary>
		/// 誰かがダメージを受けた際に呼ばれる
		/// </summary>
		/// <param name="attackerPlayerNumber">攻撃者のプレイヤー番号</param>
		/// <param name="damagedPlayerNumber">ダメージを受けたプレイヤー番号</param>
		/// <param name="damage">ダメージ量</param>
		void ExecuteDamagedEvent(int attackerPlayerNumber, int damagedPlayerNumber, int damage);


	public:
		OnlinePlayerSynchronizer(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;
		void OnLateStart() override;

		void OnUpdate() override;

		void OnDraw() override {}

		void OnDestroy() override;

		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;

		/// <summary>
		/// 移動する
		/// </summary>
		/// <param name="moveVector">移動で使用するベクトル</param>
		void Move(const Vec2& moveVector);
		/// <summary>
		/// 弾を撃つ
		/// </summary>
		void Shot();
		/// <summary>
		/// エイム状態の更新
		/// </summary>
		/// <param name="isAim">エイム状態にするならtrue</param>
		void Aim(bool isAim);
		/// <summary>
		/// テレポートする
		/// </summary>
		void Teleport();
		/// <summary>
		/// 箱を開ける
		/// </summary>
		void OpenHidePlace();

		void SetOnlinePlayerNumber(int onlinePlayerNumber) { m_onlinePlayerNumber = onlinePlayerNumber; }
		int GetOnlinePlayerNumber() const { return m_onlinePlayerNumber; }

		void SetGamePlayerNumber(int gamePlayerNumber) { m_gamePlayerNumber = gamePlayerNumber; }
		int GetGamePlayerNumber() const { return m_gamePlayerNumber; }

		/// <summary>
		/// オンラインプレイヤー番号に対応したOnlinePlayerSynchronizerを取得
		/// </summary>
		/// <param name="onlinePlayerNumber">オンラインプレイヤー番号</param>
		/// <returns>対応したOnlinePlayerSynchronizer</returns>
		static std::shared_ptr<OnlinePlayerSynchronizer> GetOnlinePlayerSynchronizer(int onlinePlayerNumber);
		/// <summary>
		/// ローカルのOnlinePlayerSynchronizerを取得
		/// </summary>
		/// <returns>ローカルのOnlinePlayerSynchronizer</returns>
		static std::shared_ptr<OnlinePlayerSynchronizer> GetLocalOnlinePlayerSynchronizer() { return GetOnlinePlayerSynchronizer(Online::OnlineManager::GetLocalPlayer().getNumber()); }
	};
}