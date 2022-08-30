#include "stdafx.h"
#include "MatchingUIController.h"
#include "../UI/SplashMessageUI.h"
#include "Zooming.h"
#include "../DebugClass/Debug.h"

namespace basecross {
	MatchingUIController::MatchingUIController(const shared_ptr<GameObject>& owner,
		const shared_ptr<UIObjectCSVBuilder>& builder)
		:OnlineComponent(owner), m_builder(builder)
	{}

	void MatchingUIController::OnCreate() {
		auto gameStart = m_builder->GetUIObject(L"GameStart");
		gameStart->SetDrawActive(false);
		auto aButton = m_builder->GetUIObject(L"AButton");
		aButton->SetDrawActive(false);
		m_builder->GetUIObject(L"WaitHost")->SetDrawActive(false);

		// 親となるオブジェクトを作成し、所定の位置へ
		auto parent = GetStage()->AddGameObject<UIObjectBase>(L"GameStartParent");
		auto gameStartPos = gameStart->GetComponent<RectTransform>()->GetPosition();
		auto aButtonPos = aButton->GetComponent<RectTransform>()->GetPosition();
		parent->GetComponent<RectTransform>()->SetPosition(Vec2(0, gameStartPos.y));

		// 親子関係をつける際、親の位置に加算されるためYだけ0にする
		gameStart->GetComponent<RectTransform>()->SetPosition(Vec2(gameStartPos.x, 0));
		aButton->GetComponent<RectTransform>()->SetPosition(Vec2(aButtonPos.x, 0));

		// 親子関係を設定
		gameStart->SetParent(parent);
		aButton->SetParent(parent);
		parent->AddComponent<Zooming>();
	}

	void MatchingUIController::OnCreateRoom() {
		m_builder->GetUIObject(L"Matching")->SetDrawActive(false);
		m_builder->GetUIObject(L"GameStart")->SetDrawActive(true);
		m_builder->GetUIObject(L"AButton")->SetDrawActive(true);
		m_builder->GetUIObject<SplashMessageUI>(L"SplashMessage")
			->SetMessage(SplashMessageUI::MessageType::CreateRoom);
	}

	void MatchingUIController::OnJoinRoom() {
		m_builder->GetUIObject(L"Matching")->SetDrawActive(false);
		m_builder->GetUIObject(L"WaitHost")->SetDrawActive(true);
		m_builder->GetUIObject<SplashMessageUI>(L"SplashMessage")
			->SetMessage(SplashMessageUI::MessageType::JoinRoom);
	}
}