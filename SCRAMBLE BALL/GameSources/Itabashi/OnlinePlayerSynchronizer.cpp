﻿#include "stdafx.h"
#include "OnlinePlayerSynchronizer.h"
#include "PlayerControlManager.h"
#include "Maruyama/Bullet/Object/BulletObjectBase.h"
#include "Maruyama/Bullet/Component/ChargeBullet.h"
#include "OnlineStatus.h"
#include "Maruyama/StageObject/HidePlace.h"
#include "Maruyama/Item/HideItem.h"
#include "Watanabe/Component/PlayerStatus.h"

/// <summary>
/// バイト配列を任意の型に変換する
/// </summary>
/// <param name="bytes">バイト配列</param>
/// <returns>変換した結果</returns>
template<class T>
T ConvertByteData(const std::uint8_t* bytes)
{
	return *(T*)bytes;
}


namespace basecross
{
	struct OnlineMoveData
	{
		Vec3 moveVector;
		Vec3 forward;

		OnlineMoveData(const Vec3& moveVector, const Vec3& forward) :
			moveVector(moveVector),
			forward(forward)
		{

		}
	};

	struct OnlineShotData
	{
		Vec3 position;
		Vec3 direction;
		std::uint32_t instanceId;

		OnlineShotData(const Vec3& position, const Vec3& direction, const std::uint32_t instanceId) :
			position(position),
			direction(direction),
			instanceId(instanceId)
		{

		}
	};

	struct OnlineBulletDestroyData
	{
		std::uint32_t instanceId;
		Vec3 position;

		OnlineBulletDestroyData(std::uint32_t instanceId, const Vec3& position) :
			instanceId(instanceId),
			position(position)
		{

		}
	};

	struct OnlineTeleportData
	{
		Vec3 teleportPosition;
		Vec3 cameraPosition;

		OnlineTeleportData(const Vec3& teleportPosition, const Vec3& cameraPosition) :
			teleportPosition(teleportPosition),
			cameraPosition(cameraPosition)
		{

		}
	};

	struct OnlineFindOjbectData
	{
		std::uint32_t objectId;
		int playerNumber;

		OnlineFindOjbectData(std::uint32_t objectId, int playerNumber) :
			objectId(objectId),
			playerNumber(playerNumber)
		{

		}
	};

	struct OnlineDamageData
	{
		int attackerPlayerNumber;
		int damagedPlayerNumber;
		int damage;

		OnlineDamageData(int attackerPlayerNumber, int damagedPlayerNumber, int damage) :
			attackerPlayerNumber(attackerPlayerNumber),
			damagedPlayerNumber(damagedPlayerNumber),
			damage(damage)
		{

		}
	};

	std::vector<std::weak_ptr<OnlinePlayerSynchronizer>> OnlinePlayerSynchronizer::m_onlinePlayerSynchronizers;

	void OnlinePlayerSynchronizer::CheckUpdateCameraForward()
	{
		auto controlManager = m_controlManager.lock();

		if (!controlManager)
		{
			return;
		}

		Vec3 forward;

		if (!controlManager->IsUpdateCameraForward(&forward))
		{
			return;
		}

		Online::OnlineManager::RaiseEvent(false, (std::uint8_t*)&forward, sizeof(Vec3), EXECUTE_CAMERA_FORWARD_EVENT_CODE);
	}

	void OnlinePlayerSynchronizer::ExecuteCameraForward(int playerNumber, const Vec3& cameraForward)
	{
		if (m_onlinePlayerNumber != playerNumber)
		{
			return;
		}

		auto controlManager = m_controlManager.lock();

		if (!controlManager)
		{
			return;
		}

		controlManager->ExecuteUpdateCameraForward(cameraForward);
	}

	void OnlinePlayerSynchronizer::ExecuteMove(int playerNumber, const Vec3& moveVector, const Vec3& forward)
	{
		if (m_onlinePlayerNumber != playerNumber)
		{
			return;
		}

		auto controlManager = m_controlManager.lock();
		controlManager->ExecuteMove(moveVector, forward);
	}

	void OnlinePlayerSynchronizer::ExecuteShot(int playerNumber, const Vec3& bulletPosition, const Vec3& bulletDirection, std::uint32_t instanceId)
	{
		if (playerNumber != m_onlinePlayerNumber)
		{
			return;
		}

		auto controlManager = m_controlManager.lock();

		auto bulletObject = controlManager->ExecuteShot(bulletDirection);

		if (!bulletObject) {
			return;
		}

		auto bulletTransform = bulletObject->GetComponent<Transform>();
		auto bulletOnlineStatus = bulletObject->GetComponent<Online::OnlineStatus>();

		bulletTransform->SetWorldPosition(bulletPosition);

		// ホストが弾の削除の管理を行う
		if (Online::OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			auto chargeBullet = bulletObject->GetComponent<ChargeBullet>();
			chargeBullet->AddDestroyEventFunc([&](const std::shared_ptr<GameObject>& gameObject) { BulletDestroyed(gameObject); });
		}
		else
		{
			auto collision = bulletObject->GetComponent<Collision>();
			collision->SetUpdateActive(false);
		}

		bulletOnlineStatus->ChangeInstanceId(instanceId);

		m_bulletObjectMap.insert(std::make_pair(bulletOnlineStatus->GetInstanceId(), bulletObject));
	}

	void OnlinePlayerSynchronizer::BulletDestroyed(const std::shared_ptr<GameObject>& destroyedGameObject)
	{
		auto onlineStatus = destroyedGameObject->GetComponent<Online::OnlineStatus>();

		auto destroyData = OnlineBulletDestroyData(onlineStatus->GetInstanceId(), destroyedGameObject->GetComponent<Transform>()->GetPosition());

		Online::OnlineManager::RaiseEvent(false, (std::uint8_t*)&destroyData, sizeof(OnlineBulletDestroyData), EXECUTE_BULLET_DESTROY_EVENT_CODE);
	}

	void OnlinePlayerSynchronizer::ExecuteBulletDestroyEvent(std::uint32_t bulletInstanceId, const Vec3& position)
	{
		auto find = m_bulletObjectMap.find(bulletInstanceId);

		if (find == m_bulletObjectMap.end())
		{
			return;
		}

		auto bulletObject = find->second.lock();
		m_bulletObjectMap.erase(bulletInstanceId);

		if (!bulletObject)
		{
			return;
		}

		auto transform = bulletObject->GetComponent<Transform>();
		transform->SetWorldPosition(position);

		bulletObject->Destroy();
	}

	void OnlinePlayerSynchronizer::ExecuteAimEvent(int playerNumber, bool isAim)
	{
		if (m_onlinePlayerNumber != playerNumber)
		{
			return;
		}

		auto controlManager = m_controlManager.lock();
		controlManager->ExecuteUpdateAim(isAim);
	}

	void OnlinePlayerSynchronizer::ExecuteTeleportEvent(int playerNumber, const Vec3& teleportPosition, const Vec3& cameraPosition)
	{
		if (m_onlinePlayerNumber != playerNumber)
		{
			return;
		}

		auto controlManager = m_controlManager.lock();

		controlManager->ExecuteTeleport(teleportPosition, cameraPosition);
	}

	void OnlinePlayerSynchronizer::TryOpenHidePlaceEvent(int playerNumber, std::uint32_t instanceId)
	{
		auto& localPlayer = Online::OnlineManager::GetLocalPlayer();

		// 自分がホストで無いか、対応したプレイヤーではないなら
		if (!localPlayer.getIsMasterClient() || m_onlinePlayerNumber != playerNumber)
		{
			return;
		}

		auto controlManager = m_controlManager.lock();
		auto hidePlace = Online::OnlineStatus::FindOnlineGameObject(instanceId)->GetComponent<HidePlace>();

		if (!controlManager->TryAccessHidePlace(hidePlace))
		{
			return;
		}

		Online::OnlineManager::RaiseEvent(false, (std::uint8_t*)&OnlineFindOjbectData(instanceId, playerNumber), sizeof(OnlineFindOjbectData), EXECUTE_OPEN_HIDEPLACE_EVENT_CODE);

		auto hideItem = hidePlace->TakeOutHideItem();

		if (!hideItem)
		{
			return;
		}

		controlManager->TryAquisition(hideItem->GetItem());
	}

	void OnlinePlayerSynchronizer::ExecuteOpenHidePlace(int playerNumber, std::uint32_t instanceId)
	{
		if (m_onlinePlayerNumber != playerNumber)
		{
			return;
		}

		auto controlManager = m_controlManager.lock();

		auto onlineGameObject = Online::OnlineStatus::FindOnlineGameObject(instanceId);
		auto hidePlace = onlineGameObject->GetComponent<HidePlace>();
		controlManager->TryAccessHidePlace(hidePlace);

		auto hideItem = hidePlace->TakeOutHideItem();

		if (!hideItem)
		{
			return;
		}

		controlManager->TryAquisition(hideItem->GetItem());
	}

	void OnlinePlayerSynchronizer::Damaged(const std::shared_ptr<PlayerStatus>& playerStatus, const DamageData& damageData)
	{
		if (!Online::OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			return;
		}

		auto attackerOnlineController = damageData.attacker->GetComponent<OnlinePlayerSynchronizer>();
		auto damagedOnlineController = playerStatus->GetGameObject()->GetComponent<OnlinePlayerSynchronizer>();
		OnlineDamageData data(attackerOnlineController->GetOnlinePlayerNumber(), damagedOnlineController->GetOnlinePlayerNumber(), damageData.value);

		Online::OnlineManager::RaiseEvent(false, (std::uint8_t*)&data, sizeof(OnlineDamageData), EXECUTE_DAMAGE_EVENT_CODE);
	}

	void OnlinePlayerSynchronizer::ExecuteDamagedEvent(int attackerPlayerNumber, int damagedPlayerNumber, int damage)
	{
		auto playerStatus = m_playerStatus.lock();

		if (!playerStatus || m_onlinePlayerNumber != damagedPlayerNumber)
		{
			return;
		}

		auto attackerOnlineController = GetOnlinePlayerSynchronizer(attackerPlayerNumber);

		playerStatus->AddDamage(DamageData(damage, attackerOnlineController->GetGameObject()));
	}

	OnlinePlayerSynchronizer::OnlinePlayerSynchronizer(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{

	}

	void OnlinePlayerSynchronizer::OnCreate()
	{
		m_onlinePlayerSynchronizers.push_back(GetThis<OnlinePlayerSynchronizer>());
	}

	void OnlinePlayerSynchronizer::OnLateStart()
	{
		auto& owner = GetGameObject();

		auto playerStatus = owner->GetComponent<PlayerStatus>();
		auto damageFunc = [&](const std::shared_ptr<PlayerStatus>& playerStatus, const DamageData& damageData) {Damaged(playerStatus, damageData); };
		playerStatus->AddFuncAddDamage(damageFunc);
		m_playerStatus = owner->GetComponent<PlayerStatus>();

		m_controlManager = owner->GetComponent<PlayerControlManager>();
	}

	void OnlinePlayerSynchronizer::OnUpdate()
	{
		CheckUpdateCameraForward();
	}

	void OnlinePlayerSynchronizer::OnDestroy()
	{
		auto thisPtr = GetThis<OnlinePlayerSynchronizer>();

		auto it = m_onlinePlayerSynchronizers.begin();

		while (it != m_onlinePlayerSynchronizers.end())
		{
			if (it->lock() == thisPtr)
			{
				m_onlinePlayerSynchronizers.erase(it);
				return;
			}
		}
	}

	void OnlinePlayerSynchronizer::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		// オンラインプレイヤー番号が無効なら
		if (playerNumber == Online::OnlineManager::INVALID_ONLINE_PLAYER_NUMBER)
		{
			return;
		}

		// カメラの前方ベクトル変更が通知されたら
		if (eventCode == EXECUTE_CAMERA_FORWARD_EVENT_CODE)
		{
			auto cameraForward = ConvertByteData<Vec3>(bytes);
			ExecuteCameraForward(playerNumber, cameraForward);
			return;
		}
		// 移動イベントが通知されたら
		if (eventCode == EXECUTE_MOVE_EVENT_CODE)
		{
			auto data = ConvertByteData<OnlineMoveData>(bytes);
			ExecuteMove(playerNumber, data.moveVector, data.forward);
			return;
		}
		// 弾を撃つイベントが通知されたら
		if (eventCode == EXECUTE_SHOT_EVENT_CODE)
		{
			auto data = ConvertByteData<OnlineShotData>(bytes);
			ExecuteShot(playerNumber, data.position, data.direction, data.instanceId);
			return;
		}
		// 弾を破棄するイベントが通知されたら
		if (eventCode == EXECUTE_BULLET_DESTROY_EVENT_CODE)
		{
			auto data = ConvertByteData<OnlineBulletDestroyData>(bytes);
			ExecuteBulletDestroyEvent(data.instanceId, data.position);
			return;
		}
		// エイムの変更イベントが通知されたら
		if (eventCode == EXECUTE_AIM_STATE_CHANGE_EVENT_CODE)
		{
			bool isAim = ConvertByteData<bool>(bytes);
			ExecuteAimEvent(playerNumber, isAim);
			return;
		}
		// テレポートしたイベントが通知されたら
		if (eventCode == EXECUTE_TELEPORT_EVENT_CODE)
		{
			auto data = ConvertByteData<OnlineTeleportData>(bytes);
			ExecuteTeleportEvent(playerNumber, data.teleportPosition, data.cameraPosition);
			return;
		}
		// 箱を開けるのを試すイベントが通知されたら
		if (eventCode == TRY_OPEN_HIDEPLACE_EVENT_CODE)
		{
			std::uint32_t instanceId = ConvertByteData<std::uint32_t>(bytes);
			TryOpenHidePlaceEvent(playerNumber, instanceId);
			return;
		}
		// 箱を開けたイベントが通知されたら
		if (eventCode == EXECUTE_OPEN_HIDEPLACE_EVENT_CODE)
		{
			auto findGameObjectData = ConvertByteData<OnlineFindOjbectData>(bytes);
			ExecuteOpenHidePlace(findGameObjectData.playerNumber, findGameObjectData.objectId);
			return;
		}
		// ダメージを受けたイベントが通知されたら
		if (eventCode == EXECUTE_DAMAGE_EVENT_CODE)
		{
			auto data = ConvertByteData<OnlineDamageData>(bytes);
			ExecuteDamagedEvent(data.attackerPlayerNumber, data.damagedPlayerNumber, data.damage);
			return;
		}
	}

	void OnlinePlayerSynchronizer::Move(const Vec2& moveVector)
	{
		auto controlManager = m_controlManager.lock();

		if (!controlManager)
		{
			return;
		}

		Vec3 resultMoveVector;
		Vec3 forward;

		if (!controlManager->TryMove(moveVector, &resultMoveVector, &forward))
		{
			return;
		}

		if (resultMoveVector != m_beforeMoveVector)
		{
			Online::OnlineManager::RaiseEvent(false, (std::uint8_t*)&OnlineMoveData(resultMoveVector, forward), sizeof(OnlineMoveData), EXECUTE_MOVE_EVENT_CODE);
		}

		m_beforeMoveVector = moveVector;
	}

	void OnlinePlayerSynchronizer::Shot()
	{
		auto controlManager = m_controlManager.lock();

		if (!controlManager)
		{
			return;
		}

		std::shared_ptr<BulletObjectBase> bulletObject;

		if (!controlManager->TryShot(&bulletObject))
		{
			return;
		}

		auto bulletOnlineStatus = bulletObject->GetComponent<Online::OnlineStatus>();

		// ホストが弾の削除の管理を行う
		if (Online::OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			auto chargeBullet = bulletObject->GetComponent<ChargeBullet>();
			chargeBullet->AddDestroyEventFunc([&](const std::shared_ptr<GameObject>& gameObject) { BulletDestroyed(gameObject); });
		}
		else
		{
			auto collision = bulletObject->GetComponent<Collision>();
			collision->SetUpdateActive(false);
		}

		auto bulletTransform = bulletObject->GetComponent<Transform>();

		bulletOnlineStatus->SetCreateInstanceId();

		m_bulletObjectMap.insert(std::make_pair(bulletOnlineStatus->GetInstanceId(), bulletObject));

		auto shotData = OnlineShotData(bulletTransform->GetWorldPosition(), transform->GetForward(), bulletOnlineStatus->GetInstanceId());

		Online::OnlineManager::RaiseEvent(false, (std::uint8_t*)&shotData, sizeof(OnlineShotData), EXECUTE_SHOT_EVENT_CODE);
	}

	void OnlinePlayerSynchronizer::Aim(bool isAim)
	{
		// エイム状態が変更ないなら
		if (isAim == m_isBeforeAim)
		{
			return;
		}

		m_isBeforeAim = isAim;

		auto controlManager = m_controlManager.lock();

		if (!controlManager)
		{
			return;
		}

		if (!controlManager->TryUpdateAim(isAim))
		{
			return;
		}

		Online::OnlineManager::RaiseEvent(false, (std::uint8_t*)&isAim, sizeof(bool), EXECUTE_AIM_STATE_CHANGE_EVENT_CODE);
	}

	void OnlinePlayerSynchronizer::Teleport()
	{
		auto controlManager = m_controlManager.lock();

		if (!controlManager)
		{
			return;
		}

		Vec3 teleportPosition;
		Vec3 cameraPosition;

		if (!controlManager->TryTeleport(&teleportPosition, &cameraPosition))
		{
			Online::OnlineManager::RaiseEvent(false, (std::uint8_t*)&OnlineTeleportData(teleportPosition, cameraPosition),
				sizeof(OnlineTeleportData), EXECUTE_TELEPORT_EVENT_CODE);

			return;
		}
	}

	void OnlinePlayerSynchronizer::OpenHidePlace()
	{
		auto controlManager = m_controlManager.lock();

		if (!controlManager)
		{
			return;
		}

		std::shared_ptr<HidePlace> hidePlace = controlManager->GetCanOpenHidePlace();

		if (!hidePlace)
		{
			return;
		}

		auto onlineStatus = hidePlace->GetGameObject()->GetComponent<Online::OnlineStatus>();
		std::uint32_t instanceId = onlineStatus->GetInstanceId();

		// ホストではないのなら
		if (!Online::OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			Online::OnlineManager::RaiseEvent(false, (std::uint8_t*)&instanceId, sizeof(std::uint32_t), TRY_OPEN_HIDEPLACE_EVENT_CODE);
			return;
		}

		if (!controlManager->TryAccessHidePlace(hidePlace))
		{
			return;
		}

		Online::OnlineManager::RaiseEvent(false, (std::uint8_t*)&OnlineFindOjbectData(instanceId, m_onlinePlayerNumber), sizeof(OnlineFindOjbectData), EXECUTE_OPEN_HIDEPLACE_EVENT_CODE);

		auto hideItem = hidePlace->TakeOutHideItem();

		if (!hideItem)
		{
			return;
		}

		controlManager->TryAquisition(hideItem->GetItem());
	}

	std::shared_ptr<OnlinePlayerSynchronizer> OnlinePlayerSynchronizer::GetOnlinePlayerSynchronizer(int onlinePlayerNumber)
	{
		for (const auto& weakOnlinePlayerSynchronizer : m_onlinePlayerSynchronizers)
		{
			auto onlinePlayerSynchronizer = weakOnlinePlayerSynchronizer.lock();

			if (!onlinePlayerNumber)
			{
				continue;
			}

			if (onlinePlayerSynchronizer->GetOnlinePlayerNumber() == onlinePlayerNumber)
			{
				return onlinePlayerSynchronizer;
			}
		}

		return nullptr;
	}
}