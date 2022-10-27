#pragma once
#include "OnlineManager.h"
#include <unordered_map>

namespace basecross
{
	class RotationController;
	class VelocityManager;
	class ChargeGun;
	class PlayerStatus;
	class ChargeBullet;
	class UseWeapon;

	class PlayerControlManager;

	struct DamageData;

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

		/// <summary>
		/// アイテムが取得できるかを問うオンラインイベントコード
		/// </summary>
		static constexpr std::uint8_t TRY_ACQUISITION_EVENT_CODE = 2;
		/// <summary>
		/// アイテムの取得を実行するオンラインイベントコード
		/// </summary>
		static constexpr std::uint8_t EXECUTE_ACQUISITION_EVENT_CODE = 3;
		static constexpr std::uint8_t EXECUTE_MOVE_EVENT_CODE = 6;
		static constexpr std::uint8_t EXECUTE_SHOT_EVENT_CODE = 7;
		static constexpr std::uint8_t EXECUTE_DAMAGE_EVENT_CODE = 8;
		static constexpr std::uint8_t EXECUTE_BULLET_DESTROY_EVENT_CODE = 11;
		static constexpr std::uint8_t EXECUTE_AIM_STATE_CHANGE_EVENT_CODE = 12;
		static constexpr std::uint8_t EXECUTE_CAMERA_FORWARD_EVENT_CODE = 13;
		static constexpr std::uint8_t EXECUTE_TELEPORT_EVENT_CODE = 14;

		static constexpr std::uint8_t TRY_OPEN_HIDEPLACE_EVENT_CODE = 15;
		static constexpr std::uint8_t EXECUTE_OPEN_HIDEPLACE_EVENT_CODE = 16;

	private:

		std::weak_ptr<Transform> m_transform;
		/// <summary>
		/// オブジェクトを動かす用のコンポーネント
		/// </summary>
		std::weak_ptr<Operator::ObjectMover> m_objectMover;
		/// <summary>
		/// オブジェクトの回転制御用コンポーネント
		/// </summary>
		std::weak_ptr<RotationController> m_rotationController;

		std::weak_ptr<VelocityManager> m_velocityManager;

		std::weak_ptr<ChargeGun> m_chargeGun;

		std::weak_ptr<PlayerStatus> m_playerStatus;

		std::weak_ptr<UseWeapon> m_useWepon;

		std::weak_ptr<PlayerControlManager> m_controlManager;

		/// <summary>
		/// 対応するプレイヤー番号
		/// </summary>
		int m_onlinePlayerNumber = OnlineManager::INVALID_ONLINE_PLAYER_NUMBER;

		int m_gamePlayerNumber = 0;

		std::unordered_map<int, std::shared_ptr<ChargeBullet>> m_chargeBulletMap;

		Vec3 m_beforeMoveVector = Vec3();

		void UpdateCameraForward();

		void ExecuteCameraForward(int playerNumber, const Vec3& cameraForward);

		void Move();

		void ExecuteMove(int playerNumber, const Vec3& moveVector, const Vec3& forward);

		/// <summary>
		/// アイテムの取得を試みる
		/// </summary>
		void TryAquisition();
		/// <summary>
		/// アイテムの取得の問い合わせの時に呼ばれるイベント
		/// </summary>
		/// <param name="itemId">取得できるか問い合わせるアイテムID</param>
		/// <param name="playerNumber">取得したいプレイヤー番号</param>
		void TryAcquisitionEvent(std::uint32_t itemId, int playerNumber);
		/// <summary>
		/// 誰かがアイテムを取得した際に呼ばれるイベント
		/// </summary>
		/// <param name="itemId">取得するアイテムID</param>
		/// <param name="playerNumber">取得するプレイヤー番号</param>
		void ExecuteAcquisitionEvent(std::uint32_t itemId, int playerNumber);

		/// <summary>
		/// 弾を撃つ
		/// </summary>
		void Shot();
		/// <summary>
		/// 誰か(自分以外)が弾を撃った際に呼ばれるイベント
		/// </summary>
		/// <param name="playerNumber">撃ったプレイヤー番号</param>
		/// <param name="bulletPosition">発射位置</param>
		/// <param name="bulletDirection">発射方向</param>
		/// <param name="instanceId">弾のID</param>
		void ExecuteShot(int playerNumber, const Vec3& bulletPosition, const Vec3& bulletDirection, int instanceId);
		/// <summary>
		/// 弾の削除を行う
		/// </summary>
		/// <param name="destroyedGameObject">削除する弾のオブジェクト</param>
		void BulletDestroyed(const std::shared_ptr<GameObject>& destroyedGameObject);
		/// <summary>
		/// 弾が削除されたときに呼ばれるイベント
		/// </summary>
		/// <param name="bulletInstanceId">削除する弾のID</param>
		void ExecuteBulletDestroyEvent(int bulletInstanceId);
		/// <summary>
		/// ダメージを受けた
		/// </summary>
		/// <param name="playerStatus">プレイヤーのステータス</param>
		/// <param name="damageData">ダメージ情報</param>
		void Damaged(const std::shared_ptr<PlayerStatus>& playerStatus, const DamageData& damageData);
		/// <summary>
		/// 誰か(自分以外)がダメージを受けた際に呼ばれる
		/// </summary>
		/// <param name="attackerPlayerNumber">攻撃者のプレイヤー番号</param>
		/// <param name="damagedPlayerNumber">ダメージを受けたプレイヤー番号</param>
		/// <param name="damage">ダメージ量</param>
		void ExecuteDamagedEvent(int attackerPlayerNumber, int damagedPlayerNumber, int damage);

		/// <summary>
		/// エイムを試す
		/// </summary>
		void TryAim();
		/// <summary>
		/// 誰か(自分以外)がエイム状態を変更したときに呼ばれるイベント
		/// </summary>
		/// <param name="playerNumber">エイムを変更したプレイヤー番号</param>
		/// <param name="isAim">エイム中か</param>
		void ExecuteAimEvent(int playerNumber, bool isAim);
		/// <summary>
		/// インスタンスIDの発行
		/// </summary>
		/// <returns>生成したインスタンスid</returns>
		int CreateInstanceId() const;
		/// <summary>
		/// テレポート入力
		/// </summary>
		void TeleportInputer();
		/// <summary>
		/// 誰かがテレポートしたら呼ばれるイベント
		/// </summary>
		/// <param name="playerNumber">テレポートしたプレイヤー番号</param>
		/// <param name="teleportPosition">テレポート位置</param>
		/// <param name="cameraPosition">カメラ位置</param>
		void ExecuteTeleportEvent(int playerNumber, const Vec3& teleportPosition, const Vec3& cameraPosition);

		void OpenHidePlaceInputer();

		void TryOpenHidePlaceEvent(int playerNumber, std::uint32_t instanceId);

		void ExecuteOpenHidePlace(int playerNumber, std::uint32_t instanceId);

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
		void SetOnlinePlayerNumber(int playerNumber) { m_onlinePlayerNumber = playerNumber; }
		/// <summary>
		/// 対応するプレイヤー番号の取得
		/// </summary>
		/// <returns>プレイヤー番号</returns>
		int GetOnlinePlayerNumber() const { return m_onlinePlayerNumber; }

		void SetGamePlayerNumber(int gamePlayerNumber) { m_gamePlayerNumber = gamePlayerNumber; }

		int GetGamePlayerNumber() const { return m_gamePlayerNumber; }

		std::shared_ptr<PlayerControlManager> GetPlayerControlManager() const { return m_controlManager.lock(); }

		/// <summary>
		/// プレイヤー番号に対応したPlayerOnlineControllerを取得
		/// </summary>
		/// <param name="playerNumber">プレイヤー番号</param>
		/// <returns>対応したPlayerOnlineController</returns>
		static std::shared_ptr<PlayerOnlineController> GetPlayerOnlineController(int playerNumber);
		/// <summary>
		/// ローカルPlayerOnlineControllerを取得
		/// </summary>
		/// <returns>ローカルPlayerOnlineController</returns>
		static std::shared_ptr<PlayerOnlineController> GetLocalOnlineController() { return GetPlayerOnlineController(Online::OnlineManager::GetLocalPlayer().getNumber()); }
	};
}
}