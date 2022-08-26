#include "stdafx.h"
#include "MatchingUIController.h"
#include "../UI/SplashMessageUI.h"

namespace basecross {
	MatchingUIController::MatchingUIController(const shared_ptr<GameObject>& owner,
		const shared_ptr<UIObjectCSVBuilder>& builder)
		:OnlineComponent(owner), m_builder(builder)
	{}

	void MatchingUIController::OnCreate() {
		m_builder->GetUIObject(L"GameStart")->SetActive(false);
		m_builder->GetUIObject(L"WaitHost")->SetActive(false);
	}

	void MatchingUIController::OnCreateRoom() {
		m_builder->GetUIObject(L"Matching")->SetActive(false);
		m_builder->GetUIObject(L"GameStart")->SetActive(true);
		m_builder->GetUIObject<SplashMessageUI>(L"SplashMessage")
			->SetMessage(SplashMessageUI::MessageType::CreateRoom);
	}

	void MatchingUIController::OnJoinRoom() {
		m_builder->GetUIObject(L"Matching")->SetActive(false);
		m_builder->GetUIObject(L"WaitHost")->SetActive(true);
		m_builder->GetUIObject<SplashMessageUI>(L"SplashMessage")
			->SetMessage(SplashMessageUI::MessageType::JoinRoom);
	}
}