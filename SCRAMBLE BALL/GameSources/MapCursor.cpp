
/*!
@file MapCursor.h
@brief MapCursorなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "MapCursor.h"

#include "SpriteObject.h"

#include "PlayerInputer.h"

#include "Maruyama/Player/Component/FieldMap.h"
#include "Watanabe/DebugClass/Debug.h"

#include "MaruUtility.h"
#include "Mathf.h"

#include "Maruyama/Player/Component/Teleport.h"
#include "Maruyama/Interface/I_TeamMember.h"
#include "TeleportCursorUI.h"
#include "Maruyama/Utility/SingletonComponent/GameManager.h"

namespace basecross {

	MapCursor::MapCursor(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_beforePosition(Vec3(0.0f))
		//m_moveRangeLate(0.5f)
	{}

	void MapCursor::OnCreate() {
		SettingUnderCircle();
		SettingCursor();
		SettingTeleportUI();
	}

	void MapCursor::OnUpdate() {
		if (!GetDrawActive() || !GetTarget() || !m_teleport.lock()) {
			return;
		}

		//テレポートできるなら
		if (m_teleport.lock()->CanTeleport()) {
			m_beforePosition = transform->GetPosition();
			MoveCursor();
		}
		else {
			transform->SetPosition(m_beforePosition);
		}
		
		//自陣エリア外なら、またゲーム中でないなら
		auto teamMember = m_teamMember.lock();
		auto gameManager = GameManager::GetInstance();
		if ((teamMember && !teamMember->IsInArea()) || (gameManager && gameManager->GetCurrentState() != GameManager::State::Game)) {
			m_teleportUIObject.lock()->SetDrawActive(false);
			SetMapCursorPositionConnectTargetPosition();
		}
		else {
			auto teleportUI = m_teleportUIObject.lock();
			//テレポートできて、テレポートUIが非表示なら、表示する。
			m_teleport.lock()->CanTeleport() && !teleportUI->GetDrawActive() ? teleportUI->SetDrawActive(true) : false;
		}
	}

	void MapCursor::SettingCursor() {
		auto param = Builder::VertexPCTParametor(Vec3(64.0f, 128.0f, 0.0f) * 0.5f, Vec2(64.0f, 128.0f), L"Cursor_TX");
		param.pivot = Builder::UIPivot::Down;
		auto cursor = GetStage()->AddGameObject<SpriteObject>(param);
		cursor->SetParent(GetGameObject());
		cursor->GetComponent<Transform>()->SetPosition(Vec3(0.0f));

		cursor->SetDrawLayer(UI::Layer::MAP_CURSOR);
		m_cursor = cursor;
	}

	void MapCursor::SettingUnderCircle() {
		auto param = Builder::VertexPCTParametor(Vec3(128.0f, 64.0f, 0.0f) * 0.5f, Vec2(512.0f, 256.0f), L"MapCursorUnderCircle_TX");
		auto underCircle = GetStage()->AddGameObject<SpriteObject>(param);
		underCircle->SetParent(GetGameObject());
		underCircle->GetComponent<Transform>()->SetPosition(Vec3(0.0f));

		underCircle->SetDrawLayer(UI::Layer::MAP_CURSOR);
		m_underCircle = underCircle;
	}

	void MapCursor::SettingTeleportUI() {
		auto uiObject = GetStage()->AddGameObject<GameObject>();
		uiObject->SetParent(GetGameObject());
		auto teleport = uiObject->AddComponent<TeleportCursorUI>();

		auto uiTrans = uiObject->GetComponent<Transform>();
		uiTrans->SetPosition(Vec3(-60.0f, -30.0f, 0.0f));

		m_teleportUIObject = uiObject;
	}

	void MapCursor::MoveCursor() {
		auto moveVec2 = PlayerInputer::GetInstance()->GetRSticVec();
		auto moveVec = Vec3(moveVec2.x, moveVec2.y, 0.0f);
		auto position = transform->GetPosition();
		constexpr float Speed = 500.0f;
		position += moveVec * Speed * App::GetApp()->GetElapsedTime();
		position = MoveClamp(position);
		transform->SetPosition(position);

		auto teleport = m_teleport.lock();
		if (teleport && !teleport->CanTeleport()) {
			transform->SetPosition(m_beforePosition);
		}
	}

	Vec3 MapCursor::MoveClamp(const Vec3& position) {
		auto mapScale = FieldMap::GetInstance()->GetMapTextureScale();
		maru::Rect rect;
		rect.startPosition = Vec3(0.0f);
		rect.width = mapScale.x;
		rect.depth = mapScale.y;
		auto rectStartPosition = rect.CalculateStartPosition();

		float x = maru::Mathf::Clamp(position.x, rectStartPosition.x, rectStartPosition.x + rect.width);
		float y = maru::Mathf::Clamp(position.y, rectStartPosition.z, rectStartPosition.z + rect.depth);

		return Vec3(x, y, position.z);
	}

	void MapCursor::SetMapCursorPositionConnectTargetPosition() {
		auto teamMember = GetTarget()->GetComponent<I_TeamMember>(false);
		int value = 1;
		if (teamMember->GetTeam() == team::TeamType::Red) {
			value = -1;
		}

		auto rect = FieldMap::GetInstance()->GetRect();
		auto halfMapTextureScale = FieldMap::GetInstance()->GetMapTextureScale() * 0.5f;
		auto startPosition = GetTarget()->GetComponent<Transform>()->GetPosition();
		float xRate = startPosition.x / (rect.width * 0.5f);
		float yRate = startPosition.z / (rect.depth * 0.5f);
		auto position = Vec3(halfMapTextureScale.x * xRate, halfMapTextureScale.y * yRate, 0.0f);

		transform->SetPosition(position * (float)value);
	}

	void MapCursor::SetTarget(const std::shared_ptr<GameObject>& target) { 
		m_target = target; 

		m_teleport = target->GetComponent<Teleport>(false);
		m_teamMember = target->GetComponent<I_TeamMember>(false);
	}

	Vec3 MapCursor::GetCursorFiledPosition() {
		auto teamMember = GetTarget()->GetComponent<I_TeamMember>(false);
		int value = 1;
		if (teamMember->GetTeam() == team::TeamType::Red) {
			value = -1;
		}

		auto sprite = m_cursor.lock();
		auto spriteTrans = sprite->GetComponent<Transform>();

		auto rect = FieldMap::GetInstance()->GetRect();
		auto mapTextureScale = FieldMap::GetInstance()->GetMapTextureScale();
		float halfWidth = rect.width * 0.5f;
		float halfDepth = rect.depth * 0.5f;
		auto startPosition = transform->GetPosition();
		float xRate = startPosition.x / (mapTextureScale.x * 0.5f);
		float yRate = startPosition.y / (mapTextureScale.y * 0.5f);
		auto position = (Vec3(halfWidth * xRate, 0.1f, halfDepth * yRate));

		return position * (float)value;
	}

	void MapCursor::OnDrawActive() {
		m_cursor.lock()->SetDrawActive(true);
		m_underCircle.lock()->SetDrawActive(true);

		if (m_teleport.lock() && m_teleport.lock()->CanTeleport()) {
			m_teleportUIObject.lock()->SetDrawActive(true);
		}

		SetMapCursorPositionConnectTargetPosition();
		m_beforePosition = transform->GetPosition();
	}

	void MapCursor::OnDrawFalse() {
		m_cursor.lock()->SetDrawActive(false);
		m_underCircle.lock()->SetDrawActive(false);
		m_teleportUIObject.lock()->SetDrawActive(false);
	}

}