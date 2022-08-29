
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

namespace basecross {

	MapCursor::MapCursor(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr)
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
			//Debug::GetInstance()->Log(transform->GetWorldPosition());

			MoveCursor();
		}
	}

	void MapCursor::MoveCursor() {
		auto moveVec2 = PlayerInputer::GetInstance()->GetRSticVec();
		auto moveVec = Vec3(moveVec2.x, moveVec2.y, 0.0f);
		auto position = transform->GetPosition();
		constexpr float Speed = 500.0f;
		position += moveVec * Speed * App::GetApp()->GetElapsedTime();
		transform->SetPosition(position);
	}

	Vec3 MapCursor::GetCursorFiledPosition() {
		auto sprite = m_sprite.lock();
		auto spriteTrans = sprite->GetComponent<Transform>();

		auto rect = FieldMap::GetInstance()->GetRect();
		auto startPosition = transform->GetPosition();
		float xRate = startPosition.x / (256.0f);
		float yRate = startPosition.y / (512.0f);
		auto position = (Vec3(50.0f * xRate, 0.1f, 100.0f * yRate));

		return position;
	}

	void MapCursor::OnDrawActive() {
		m_sprite.lock()->SetDrawActive(true);

		auto sprite = m_sprite.lock();
		//auto spriteTrans = sprite->GetComponent<Transform>();

		auto rect = FieldMap::GetInstance()->GetRect();
		auto startPosition = GetTarget()->GetComponent<Transform>()->GetPosition();
		float xRate = startPosition.x / (rect.width * 0.5f);
		float yRate = startPosition.z / (rect.depth * 0.5f);
		transform->SetPosition(Vec3(256.0f * xRate, 512.0f * yRate, 0.0f));
	}

	void MapCursor::OnDrawFalse() {
		m_sprite.lock()->SetDrawActive(false);
	}

}