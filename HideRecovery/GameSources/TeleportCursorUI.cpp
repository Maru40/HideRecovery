
/*!
@file TeleportCursorUI.cpp
@brief TeleportCursorUIクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TeleportCursorUI.h"

#include "SpriteObject.h"

#include "Watanabe/UI/ButtonUI.h"

namespace basecross {

	TeleportCursorUI::TeleportCursorUI(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr)
	{}

	void TeleportCursorUI::OnCreate() {
		SettingBody();
		SettingBottun();

		//transform->SetPosition(Vec3(450.0f, -500.0f, 0.0f));
		transform->SetScale(Vec3(0.25f));
	}

	void TeleportCursorUI::OnEnable() {
		m_body.lock()->SetDrawActive(true);
		m_bottun.lock()->SetDrawActive(true);
	}

	void TeleportCursorUI::OnDisable() {
		m_body.lock()->SetDrawActive(false);
		m_bottun.lock()->SetDrawActive(false);
	}

	void TeleportCursorUI::SettingBody() {
		auto param = Builder::VertexPCTParametor(Vec3(310.0f, 128.0f, 0.0f), 0.35f, Col4(1.0f),
			Builder::UseUVRange(0.0f, 310.0f), Builder::UseUVRange(0.0f, 128.0f), Vec2(640.0f, 128.0f), L"TeleportUI_TX", Builder::UIPivot::Left);
		auto body = GetStage()->AddGameObject<SpriteObject>(param);
		body->SetDrawLayer(UI::Layer::MAP_CURSOR);

		body->SetParent(GetGameObject());

		auto bodyTrans = body->GetComponent<Transform>();
		bodyTrans->SetPosition(Vec3(10.0f, 0.0f, 0.0f));

		m_body = body;
	}

	void TeleportCursorUI::SettingBottun() {
		auto bottun = GetStage()->AddGameObject<ButtonUI>();
		bottun->ChangeButton(L"A");

		bottun->SetParent(GetGameObject());

		constexpr float Size = 0.35f;
		bottun->GetComponent<RectTransform>()->SetScale(Vec2(0.7f) * Size);

		m_bottun = bottun;
	}
}