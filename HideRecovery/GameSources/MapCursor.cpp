
/*!
@file MapCursor.h
@brief MapCursor‚È‚Ç
’S“–FŠÛR—TŠì
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

namespace basecross {

	MapCursor::MapCursor(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr)
		//m_moveRangeLate(0.5f)
	{}

	void MapCursor::OnCreate() {
		auto param = Builder::VertexPCTParametor(Vec3(64.0f, 128.0f, 0.0f) * 0.5f, Vec2(64.0f, 128.0f), L"Cursor_TX");
		param.pivot = Builder::UIPivot::Down;
		auto cursor = GetStage()->AddGameObject<SpriteObject>(param);
		cursor->SetParent(GetGameObject());
		cursor->GetComponent<Transform>()->SetPosition(Vec3(0.0f));

		auto pos = cursor->GetComponent<Transform>()->GetPosition();
		m_sprite = cursor;
	}

	void MapCursor::OnUpdate() {
		if (GetDrawActive()) {
			MoveCursor();
		}
	}

	void MapCursor::MoveCursor() {
		auto moveVec2 = PlayerInputer::GetInstance()->GetRSticVec();
		auto moveVec = Vec3(moveVec2.x, moveVec2.y, 0.0f);
		auto position = transform->GetPosition();
		constexpr float Speed = 500.0f;
		position += moveVec * Speed * App::GetApp()->GetElapsedTime();
		position = MoveClamp(position);
		transform->SetPosition(position);
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

	Vec3 MapCursor::GetCursorFiledPosition() {
		auto sprite = m_sprite.lock();
		auto spriteTrans = sprite->GetComponent<Transform>();

		auto rect = FieldMap::GetInstance()->GetRect();
		auto mapTextureScale = FieldMap::GetInstance()->GetMapTextureScale();
		float halfWidth = rect.width * 0.5f;
		float halfDepth = rect.depth * 0.5f;
		auto startPosition = transform->GetPosition();
		float xRate = startPosition.x / (mapTextureScale.x * 0.5f);
		float yRate = startPosition.y / (mapTextureScale.y * 0.5f);
		auto position = (Vec3(halfWidth * xRate, 0.1f, halfDepth * yRate));

		return position;
	}

	void MapCursor::OnDrawActive() {
		m_sprite.lock()->SetDrawActive(true);

		auto sprite = m_sprite.lock();

		auto rect = FieldMap::GetInstance()->GetRect();
		auto halfMapTextureScale = FieldMap::GetInstance()->GetMapTextureScale() * 0.5f;
		auto startPosition = GetTarget()->GetComponent<Transform>()->GetPosition();
		float xRate = startPosition.x / (rect.width * 0.5f);
		float yRate = startPosition.z / (rect.depth * 0.5f);
		transform->SetPosition(Vec3(halfMapTextureScale.x * xRate, halfMapTextureScale.y * yRate, 0.0f));
	}

	void MapCursor::OnDrawFalse() {
		m_sprite.lock()->SetDrawActive(false);
	}

}