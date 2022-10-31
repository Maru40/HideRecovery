#include "stdafx.h"
#include "DirectionWithHasBallUI.h"
#include "../Manager/SpriteDataManager.h"
#include "../Utility/Utility.h"

#include "../DebugClass/Debug.h"

namespace basecross {
	DirectionWithHasBallUI::DirectionWithHasBallUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"DirectionWithHasBallUI")
	{}

	shared_ptr<SimpleSprite> DirectionWithHasBallUI::CreateSprite(const wstring& spriteKey) {
		auto sprite = GetStage()->AddGameObject<SimpleSprite>(
			SimpleSprite::Type::SpriteData, spriteKey
			);
		return sprite;
	}

	void DirectionWithHasBallUI::OnCreate() {
		m_cursor = CreateSprite(L"Triangle");
		auto rectTrans = m_cursor->GetRectTransform();
		rectTrans->SetPosition(Vec2(400, 0));
		//rectTrans->SetScale(0.3f, 0.5f);
		m_cursor->SetParent(GetThis<DirectionWithHasBallUI>());
	}
	void DirectionWithHasBallUI::OnUpdate() {
	}
}