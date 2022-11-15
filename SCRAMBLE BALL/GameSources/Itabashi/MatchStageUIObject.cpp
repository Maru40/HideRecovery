#include "MatchStageUIObject.h"
#include "Watanabe/UI/UIObjectCSVBuilder.h"
#include "MatchingSelectUIObject.h"
#include "Watanabe/Component/MatchingUIController.h"
#include "Watanabe/UI/PasscodeUI.h"
#include "Itabashi/UICancelEventer.h"
#include "Itabashi/PasswordTextUI.h"
#include "Itabashi/MatchStageReconnectUIObject.h"

namespace basecross
{
	namespace StageObject
	{
		MatchStageUIObject::MatchStageUIObject(const std::shared_ptr<Stage>& stage, const std::shared_ptr<UIObjectCSVBuilder>& builder) :
			UIObject(stage),
			m_builder(builder)
		{
		}

		void MatchStageUIObject::OnCreate()
		{
			auto objectThis = GetThis<GameObject>();

			auto matchingSelectUIObject = GetStage()->AddGameObject<MatchingSelectUIObject>();
			matchingSelectUIObject->SetParent(objectThis);
			matchingSelectUIObject->SetActive(false);
			m_matchingSelectUIObject = matchingSelectUIObject;

			auto passcodeUIObject = GetStage()->AddGameObject<PasscodeUI>();
			passcodeUIObject->SetParent(objectThis);
			passcodeUIObject->SetActive(false);
			m_passcodeUIObject = passcodeUIObject;

			auto passwordViewNumbers = GetStage()->AddGameObject<Numbers>(4);
			passwordViewNumbers->SetParent(objectThis);
			passwordViewNumbers->SetActive(false);
			m_passwordViewNumbers = passwordViewNumbers;

			auto reconnectUIObject = GetStage()->AddGameObject<MatchStageReconnectUIObject>();
			reconnectUIObject->SetParent(objectThis);
			reconnectUIObject->SetActive(false);
			m_matchStageReconnectUIObject = reconnectUIObject;

			auto matchingUIController = AddComponent<MatchingUIController>(m_builder);
			matchingUIController->SetMatchingSelectUIObject(matchingSelectUIObject);
			matchingUIController->SetPasscodeUIObject(passcodeUIObject);
			matchingUIController->SetPasswordViewNumbersObject(passwordViewNumbers);
			matchingUIController->SetMatchStageReconnectUIObject(reconnectUIObject);

			auto cancelEventer = AddComponent<UICancelEventer>();

			std::weak_ptr<MatchingUIController> weakMatchingUIController = matchingUIController;

			// 部屋に入っているときにキャンセルが押されたら部屋から出るUIにする
			cancelEventer->AddCancelEvent([weakMatchingUIController]()
				{
					weakMatchingUIController.lock()->ChangeUIStartLeaveRoom();
				}
			);

			auto passwordTextCancelEventer = passcodeUIObject->GetComponent<UICancelEventer>();

			std::weak_ptr<GameObject> weakSelectUIObject = matchingSelectUIObject;
			std::weak_ptr<UI::PasswordTextUI> weakPasswordText = passcodeUIObject->GetComponent<UI::PasswordTextUI>();

			// パスワード入力状態でキャンセルが押されたら、パスワードを消してひとつ前のUIに戻る
			passwordTextCancelEventer->AddCancelEvent([weakSelectUIObject, weakPasswordText, weakMatchingUIController]()
				{
					auto matchingSelectUIObject = weakSelectUIObject.lock();
					matchingSelectUIObject->SetActive(true);

					EventSystem::GetInstance(matchingSelectUIObject->GetStage())->PopSelectableObject();

					auto passwordText = weakPasswordText.lock();
					passwordText->Clear();

					passwordText->GetGameObject()->SetActive(false);

					// スプラッシュメッセージのクリア
					weakMatchingUIController.lock()->ClearSplashMessage();
				}
			);
		}
	}
}