#pragma once
#include "OnlineManager.h"
#include <unordered_map>

namespace basecross
{
	class RotationController;
	class ItemAcquisitionManager;
	class OwnHideItemManager;
	class VelocityManager;
	class ChargeGun;
	class PlayerStatus;
	class TackleAttack;
	class ChargeBullet;
	class UseWepon;

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
		static constexpr std::uint8_t EXECUTE_SHOT_EVENT_CODE = 7;
		static constexpr std::uint8_t EXECUTE_DAMAGE_EVENT_CODE = 8;
		static constexpr std::uint8_t EXECUTE_TACKLE_EVENT_CODE = 9;
		static constexpr std::uint8_t EXECUTE_BULLET_DESTROY_EVENT_CODE = 11;
		static constexpr std::uint8_t EXECUTE_AIM_STATE_CHANGE_EVENT_CODE = 12;
		static constexpr std::uint8_t EXECUTE_CAMERA_FORWARD_EVENT_CODE = 13;

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

		std::weak_ptr<ItemAcquisitionManager> m_acquisitionManager;

		std::weak_ptr<OwnHideItemManager> m_hideItemManager;

		std::weak_ptr<VelocityManager> m_velocityManager;

		std::weak_ptr<ChargeGun> m_chargeGun;

		std::weak_ptr<PlayerStatus> m_playerStatus;

		std::weak_ptr<TackleAttack> m_tackleAttack;

		std::weak_ptr<UseWepon> m_useWepon;

		std::weak_ptr<Camera> m_camera;

		/// <summary>
		/// 対応するプレイヤー番号
		/// </summary>
		int m_playerNumber = 0;

		int m_gamePlayerNumber = 0;

		std::unordered_map<int, std::shared_ptr<ChargeBullet>> m_chargeBulletMap;

		Vec3 m_beforeMoveVector = Vec3();

		Vec3 m_cameraForward = Vec3();

		void UpdateCameraForward();

		void ExecuteCameraForward(int playerNumber, const Vec3& cameraForward);

		void Move();

		void ExecuteMove(int playerNumber, const Vec3& moveVector);

		void TryAquisition();

		void TryAcquisitionEvent(int itemId, int playerNumber);

		void ExecuteAcquisitionEvent(const ItemOwnerShipData& ownerShipData);

		void TryItemHide();

		void TryItemHideEvent(int playerNumber);

		void ExecuteItemHideEvent(int playerNumber, const Vec3& position);

		void Shot();

		void ExecuteShot(int playerNumber, const Vec3& bulletPosition, const Vec3& bulletDirection, int instanceId);

		void BulletDestroyed(const std::shared_ptr<GameObject>& destroyedGameObject);

		void ExecuteBulletDestroyEvent(int bulletInstanceId);

		void Damaged(const std::shared_ptr<PlayerStatus>& playerStatus, const DamageData& damageData);

		void ExecuteDamagedEvent(int attackerPlayerNumber, int damagedPlayerNumber, int damage);

		void TryTackle();

		void ExecuteTackle(int playerNumber);

		void TryAim();

		void ExecuteAimEvent(int playerNumber, bool isAim);

		int CreateInstanceId() const;

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

		void SetGamePlayerNumber(int gamePlayerNumber) { m_gamePlayerNumber = gamePlayerNumber; }

		int GetGamePlayerNumber() const { return m_gamePlayerNumber; }

		void SetCamera(const std::shared_ptr<Camera>& camera) { m_camera = camera; }
		std::shared_ptr<Camera> GetCamera() const { return m_camera.lock(); }

		static std::shared_ptr<PlayerOnlineController> GetPlayerOnlineController(int playerNumber);
	};
}
}