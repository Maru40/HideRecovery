#include "stdafx.h"
#include "MatchingUIController.h"
#include "../DebugClass/Debug.h"
#include "../UI/HoldButtonUI.h"
#include "../UI/SplashMessageUI.h"
#include "Itabashi/MatchingSelectUIObject.h"
#include "Itabashi/OnlineMatching.h"
#include "Itabashi/PasswordTextUI.h"
#include "Patch/PlayerInputer.h"
#include "Watanabe/UI/PasscodeUI.h"
#include "Zooming.h"

namespace basecross {
	MatchingUIController::MatchingUIController(const shared_ptr<GameObject>& owner,
		const shared_ptr<UIObjectCSVBuilder>& builder)
		: OnlineComponent(owner), m_builder(builder) {}

	void MatchingUIController::CreateUIEvent() {
		auto joinRoomButton = m_selectUIObject.lock()->GetJoinRoomButtonObject()->GetComponent<Button>();

		auto passcodeUIObject = m_passcodeUIObject.lock();
		auto passwordTextUI = passcodeUIObject->GetComponent<UI::PasswordTextUI>();

		std::weak_ptr<GameObject> weakPasscodeUIObject = passcodeUIObject;

		// 押されたら、バスワード入力UIを表示して選択する
		joinRoomButton->AddPushEvent([weakPasscodeUIObject]() {
			auto passwordObject = weakPasscodeUIObject.lock();

			passwordObject->SetActive(true);
			EventSystem::GetInstance(passwordObject->GetStage())->PushSelectableObject(passwordObject);
			});

		// 押されたら、非表示にする
		passwordTextUI->AddSubmitEvent([&, weakPasscodeUIObject](const std::wstring& password) {
			m_isJoinRoom = true;
			weakPasscodeUIObject.lock()->SetActive(false);
			});
	}

	void MatchingUIController::OnLateStart() {
		auto gameStart = m_builder->GetUIObject(L"GameStart");
		gameStart->SetDrawActive(false);
		auto aButton = m_builder->GetUIObject(L"AButton");
		aButton->SetDrawActive(false);
		m_builder->GetUIObject(L"WaitHost")->SetDrawActive(false);
		m_builder->GetUIObject(L"InProcess")->SetDrawActive(false);

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

		auto holdA = m_builder->GetUIObject<HoldButtonUI>(L"HoldA");
		holdA->SetParent(aButton);

		auto transitioner = m_transitioner.lock();

		if (transitioner) {
			holdA->SetTransitioner(transitioner);
		}
		holdA->SetDrawActive(false);

		CreateUIEvent();
	}

	void MatchingUIController::OnUpdate() {
	}

	void MatchingUIController::OnConnected() {
		auto selectUIObject = m_selectUIObject.lock();

		selectUIObject->SetActive(true);

		EventSystem::GetInstance(GetStage())->SetNowSelectableObject(selectUIObject->GetFreeMatchingButtonObject());
	}

	void MatchingUIController::OnCreateRoom() {
		m_builder->GetUIObject(L"InProcess")->SetActive(false);
		m_builder->GetUIObject(L"GameStart")->SetDrawActive(true);
		m_builder->GetUIObject(L"AButton")->SetDrawActive(true);
		m_builder->GetUIObject(L"HoldA")->SetDrawActive(true);
		m_builder->GetUIObject<SplashMessageUI>(L"SplashMessage")
			->SetMessage(SplashMessageUI::MessageType::CreateRoom);

		EventSystem::GetInstance(GetStage())->PushSelectableObject(GetGameObject());

		std::wstring password = Online::OnlineManager::GetCurrentlyJoinedRoom().getName().cstr();

		auto numbersObject = m_passwordViewNumbersObject.lock();
		auto onlineMatching = m_onlineMatching.lock();

		if (onlineMatching->UsePassword() && numbersObject) {
			auto passwordNumber = std::stoi(password);
			numbersObject->SetNumber(passwordNumber);
			numbersObject->SetActive(true);
		}
	}

	void MatchingUIController::OnJoinRoom() {
		m_builder->GetUIObject(L"InProcess")->SetActive(false);
		m_builder->GetUIObject(L"WaitHost")->SetDrawActive(true);
		m_builder->GetUIObject<SplashMessageUI>(L"SplashMessage")
			->SetMessage(SplashMessageUI::MessageType::JoinRoom);

		EventSystem::GetInstance(GetStage())->PushSelectableObject(GetGameObject());
	}

	void MatchingUIController::OnJoinRoomFailed(int errorCode) {
		auto passcodeUIObject = m_passcodeUIObject.lock();

		passcodeUIObject->SetActive(true);
		passcodeUIObject->GetComponent<UI::PasswordTextUI>()->Clear();

		m_builder->GetUIObject(L"InProcess")->SetDrawActive(false);

		m_builder->GetUIObject<SplashMessageUI>(L"SplashMessage")
			->SetMessage(SplashMessageUI::MessageType::CanNotJoin);

		m_isJoinRoom = false;
	}

	void MatchingUIController::OnLeaveRoom() {
		auto selectUIObject = m_selectUIObject.lock();

		if (m_isJoinRoom) {
			auto eventSystem = EventSystem::GetInstance(GetStage());
			eventSystem->Clear();
			eventSystem->SetNowSelectableObject(selectUIObject->GetJoinRoomButtonObject());
		}

		selectUIObject->SetActive(true);

		m_isJoinRoom = false;
	}

	void MatchingUIController::OnMasterClientChanged(int id, int oldID)
	{
		// 前のホストがいないか、新しいホストが自分ではなかったら
		if (oldID == Online::OnlineManager::INVALID_ONLINE_PLAYER_NUMBER || Online::OnlineManager::GetLocalPlayer().getNumber() != id)
		{
			return;
		}

		m_builder->GetUIObject(L"InProcess")->SetActive(false);
		m_builder->GetUIObject(L"WaitHost")->SetDrawActive(false);

		m_builder->GetUIObject(L"GameStart")->SetDrawActive(true);
		m_builder->GetUIObject(L"AButton")->SetDrawActive(true);
		m_builder->GetUIObject(L"HoldA")->SetDrawActive(true);
		//m_builder->GetUIObject<SplashMessageUI>(L"SplashMessage")
		//	->SetMessage(SplashMessageUI::MessageType::CreateRoom);

		std::wstring password = Online::OnlineManager::GetCurrentlyJoinedRoom().getName().cstr();

		auto numbersObject = m_passwordViewNumbersObject.lock();
		auto onlineMatching = m_onlineMatching.lock();

		if (onlineMatching->UsePassword() && numbersObject) {
			auto passwordNumber = std::stoi(password);
			numbersObject->SetNumber(passwordNumber);
			numbersObject->SetActive(true);
		}
	}

	void MatchingUIController::ChangeUIStartMatching() {
		// 「マッチング中」の表示
		auto inProcessUI = m_builder->GetUIObject<InProcessUI>(L"InProcess");
		inProcessUI->SetLabel(InProcessUI::LabelType::Matching);
		inProcessUI->SetActive(true);
	}

	void MatchingUIController::ChangeUIStartLeaveRoom() {
		m_builder->GetUIObject(L"GameStart")->SetDrawActive(false);
		m_builder->GetUIObject(L"AButton")->SetDrawActive(false);
		m_builder->GetUIObject(L"HoldA")->SetDrawActive(false);
		m_builder->GetUIObject(L"WaitHost")->SetDrawActive(false);

		m_passcodeUIObject.lock()->SetActive(false);
		m_passwordViewNumbersObject.lock()->SetActive(false);
	}
}