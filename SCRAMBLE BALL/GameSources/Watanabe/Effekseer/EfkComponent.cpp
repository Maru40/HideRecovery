/*!
@file   EfkComponent.cpp
@brief  Effekseerを使用するためのコンポーネントの実体
*/

#include "stdafx.h"
#include "EfkEffect.h"

namespace basecross {
	EfkComponent::EfkComponent(const shared_ptr<GameObject>& gameObjectPtr)
		:EfkComponent(gameObjectPtr, SpaceType::Local) {}

	EfkComponent::EfkComponent(const shared_ptr<GameObject>& gameObjectPtr, SpaceType useSpace)
		: Component(gameObjectPtr), m_playSpeed(1), m_useSpace(useSpace),
		m_coordinateType(CoordinateType::Relative), m_isDebugMode(false)
	{
		// マネージャーのポインタを取得
		m_manager = EfkInterface::GetInstance()->GetManager();
	}

	void EfkComponent::OnUpdate() {
		for (auto map : m_effectDataMap) {
			auto key = map.first;
			auto data = map.second;
			// 位置を同期させるか
			if (data.IsSync) {
				SyncGameObject(key);
			}

			// ループフラグがON
			if (data.IsLoop && !IsPlaying(key)) {
				Play(key);
			}
		}
	}

	void EfkComponent::SetEffectResource(const wstring& key, const TransformData& offset, bool noStopLastEffect) {
		auto effectRes = App::GetApp()->GetResource<EfkEffectResource>(key + EfkKey);
		auto data = effectRes->GetEffectData();
		m_effectDataMap[key] = EfkData(data, offset, noStopLastEffect, false, false);
	}

	void EfkComponent::SetEffectResource(const wstring& key, const TransformData& offset) {
		SetEffectResource(key, offset, false);
	}

	void EfkComponent::SetEffectResource(const wstring& key) {
		SetEffectResource(key, TransformData(), false);
	}

	void EfkComponent::Play(const wstring& key) {
		Play(key, false);
	}

	void EfkComponent::Play(const wstring& key, bool isOwnerRotation) {
		auto& data = m_effectDataMap[key];
		if (!data.NoStopLastEffect) {
			// 前回のエフェクトを停止
			Stop(key);
		}
		Vec3 pos(0);
		switch (m_coordinateType)
		{
		case CoordinateType::Absolute:
			pos = data.Offset.Position;
			break;
		case CoordinateType::Relative:
			pos = GetGameObjectPosition() + data.Offset.Position;
			break;
		}

		// 再生
		data.Handle = m_manager->Play(data.EffectData, pos.x, pos.y, pos.z);
		// ハンドルがないと設定できないのでここで再生速度を設定
		m_manager->SetSpeed(data.Handle, m_playSpeed);
		// スケールを設定
		auto scale = data.Offset.Scale;
		m_manager->SetScale(data.Handle, scale.x, scale.y, scale.z);

		// 回転を設定
		auto rad = Utility::ConvertDegVecToRadVec(data.Offset.Rotation);
		if (isOwnerRotation)
			rad += GetGameObjectRotation();
		m_manager->SetRotation(data.Handle, rad.x, rad.y, rad.z);
		if (m_isDebugMode)
			Debug::GetInstance()->Log(L"EfkComp::Playing:" + key);
	}

	void EfkComponent::Stop(const wstring& key) {
		if (IsPlaying(key)) {
			m_effectDataMap[key].IsLoop = false;
			m_manager->StopEffect(m_effectDataMap[key].Handle);
			if (m_isDebugMode)
				Debug::GetInstance()->Log(L"EfkComp::Stop:" + key);
		}
	}
	void EfkComponent::Pause(const wstring& key) {
		auto handle = m_effectDataMap[key].Handle;
		if (IsPlaying(key))
			// ポーズ中だったら続きから再生
			m_manager->SetPaused(handle, !m_manager->GetPaused(handle));
	}

	void EfkComponent::StopAll() {
		m_manager->StopAllEffects();
		for (auto& data : m_effectDataMap) {
			data.second.IsLoop = false;
		}
	}

	//void EfkComponent::SetPlaySpeed(float speed) {
	//	m_playSpeed = speed;
	//}

	void EfkComponent::SetRotation(const wstring& key, const Vec3& rotation) {
		auto data = m_effectDataMap[key];
		m_manager->SetRotation(data.Handle, rotation.x, rotation.y, rotation.z);
	}

	//void EfkComponent::SetScale(const Vec3& scale) {
	//	m_scale = scale;
	//}

	void EfkComponent::SetPosition(const wstring& key, const Vec3& position) {
		if (!IsPlaying(key))
			return;
		auto data = m_effectDataMap[key];
		m_manager->SetLocation(data.Handle, position.x, position.y, position.z);
	}

	bool EfkComponent::IsPlaying(const wstring& key) {
		return m_manager->Exists(m_effectDataMap[key].Handle);
	}

	bool EfkComponent::IsPlaying() {
		for (auto data : m_effectDataMap) {
			// 再生中のエフェクトがあればtrue
			if (IsPlaying(data.first)) {
				return true;
			}
		}
		return false;
	}

	void EfkComponent::IsSyncGameObject(const wstring& key, bool flg) {
		m_effectDataMap[key].IsSync = flg;
		if (m_isDebugMode)
			Debug::GetInstance()->Log(L"EfkComp::Sync:" + key + L" Flag:" + (flg ? L"true" : L"false"));
	}

	void EfkComponent::PlayLoop(const wstring& key) {
		m_effectDataMap[key].IsLoop = true;
		Play(key);
	}

	Vec3 EfkComponent::GetGameObjectPosition() {
		auto transComp = GetGameObject()->GetComponent<Transform>();
		switch (m_useSpace)
		{
		case SpaceType::World:
			return transComp->GetWorldPosition();
		case SpaceType::Local:
			return transComp->GetPosition();
		default:
			return transComp->GetPosition();
		}
	}

	Vec3 EfkComponent::GetGameObjectRotation() {
		Quat quat = GetGameObject()->GetComponent<Transform>()->GetQuaternion();
		return Utility::QuaternionToEulerAngle(quat);
	}

	void EfkComponent::SetCoordinateType(CoordinateType coordinateType) {
		m_coordinateType = coordinateType;
	}

	void EfkComponent::SetDebugMode(bool flg) {
		m_isDebugMode = flg;
	}

	void EfkComponent::SyncGameObject(const wstring& key) {
		auto pos = m_effectDataMap[key].Offset.Position;
		auto ownerTrans = GetGameObject()->GetComponent<Transform>();
		auto posTarget = Utility::ParentRelativePositionToWorldPosition(ownerTrans, pos);
		SetPosition(key, posTarget);
		if (m_isDebugMode)
			Debug::GetInstance()->Log(L"EfkComp::SynchronizePosition:" + key);
	}
}