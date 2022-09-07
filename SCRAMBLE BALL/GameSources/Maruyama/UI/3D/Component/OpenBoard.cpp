/*!
@file OpenBoard.cpp
@brief OpenBoardクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "OpenBoard.h"

#include "Watanabe/UI/ButtonUI.h"

#include "Maruyama/UI/BuilderVertexPCT.h"
#include "Maruyama/UI/3D/Object/BoardObject.h"

#include "Watanabe/Manager/SpriteDataManager.h"

#include "Maruyama/Utility/Component/BillBoard.h"

namespace basecross {

	OpenBoard::OpenBoard(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr)
	{}

	void OpenBoard::OnCreate() {
		CreateStringSprite();
		CreateButton();

		auto billboard = GetGameObject()->AddComponent<BillBoard>();
		billboard->SetRotationOffset(Vec3(0.0f, XMConvertToRadians(180.0f), 0.0f));

		transform->SetScale(Vec3(0.5f));
	}

	void OpenBoard::OnEnable() {
		m_button.lock()->SetDrawActive(true);
		m_stringSprite.lock()->SetDrawActive(true);
	}

	void OpenBoard::OnDisable() {
		m_button.lock()->SetDrawActive(false);
		m_stringSprite.lock()->SetDrawActive(false);
	}

	void OpenBoard::CreateStringSprite() {
		auto param = Builder::VertexPCTParametor(
			Vec3(2.0f, 1.0f, 0.0f), 1.0f, Col4(1.0f),
			Vec2(160.0f, 96.0f), L"Open_TX", Builder::UIPivot::Center
		);
		auto body = GetStage()->AddGameObject<BoardObject>(param);

		body->SetParent(GetGameObject());
		//body->AddComponent<BillBoard>();

		auto bodyTrans = body->GetComponent<Transform>();
		bodyTrans->SetPosition(Vec3(0.6f, 0.0f, 0.0f));

		m_stringSprite = body;
	}

	void OpenBoard::CreateButton() {
		auto param = Builder::VertexPCTParametor(
			Vec3(1.0f, 1.0f, 0.0f),
			Vec2(128.0f, 128.0f), L"AButton_TX", Builder::UIPivot::Center
		);
		auto body = GetStage()->AddGameObject<BoardObject>(param);

		body->SetParent(GetGameObject());
		//body->AddComponent<BillBoard>();

		auto bodyTrans = body->GetComponent<Transform>();
		bodyTrans->SetPosition(Vec3(-0.6f, 0.0f, 0.0f));
		bodyTrans->SetScale(Vec3(0.85f));

		m_button = body;
	}

}