
/*!
@file TeleportUI.cpp
@brief TeleportUI�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "TeleportUI.h"

#include "Maruyama/UI/2D/Object/SpriteObject.h"

#include "Watanabe/UI/ButtonUI.h"
#include "Watanabe/Component/Zooming.h"

namespace basecross {

	TeleportUI::TeleportUI(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_soundClip(L"AlertSE_00", false, 1.0f)
	{}

	void TeleportUI::OnCreate() {
		//m_soundEmitter = GetGameObject()->AddComponent<SoundEmitter>();

		SettingBody();
		SettingBottun();

		transform->SetPosition(Vec3(450.0f, -500.0f, 0.0f));
		transform->SetScale(Vec3(0.25f));

		SetUpdateActive(false);
	}

	void TeleportUI::OnEnable() {
		m_body.lock()->SetDrawActive(true);
		m_bottun.lock()->SetDrawActive(true);

		//m_soundEmitter.lock()->PlaySoundClip(m_soundClip);
	}

	void TeleportUI::OnDisable() {
		m_body.lock()->SetDrawActive(false);
		m_bottun.lock()->SetDrawActive(false);
	}

	void TeleportUI::SettingBody() {
		auto param = Builder::VertexPCTParametor(Vec3(640.0f, 128.0f, 0.0f), 0.75f, Vec2(640.0f, 128.0f), L"TeleportUI_TX");
		auto body = GetStage()->AddGameObject<SpriteObject>(param);

		body->SetParent(GetGameObject());

		auto bodyTrans = body->GetComponent<Transform>();
		bodyTrans->SetPosition(Vec3(265.0f, 0.0f, 0.0f));

		m_body = body;
	}

	void TeleportUI::SettingBottun() {
		auto bottun = GetStage()->AddGameObject<ButtonUI>();
		bottun->ChangeButton(L"Y");

		bottun->SetParent(GetGameObject());

		bottun->GetComponent<RectTransform>()->SetScale(Vec2(0.7f) * 0.75f);
		bottun->AddComponent<Zooming>();

		m_bottun = bottun;
	}
}