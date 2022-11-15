#include "MatchStageReconnectUIObject.h"
#include "Watanabe/UI/SimpleSprite.h"
#include "Itabashi/SelectAppearanceChanger.h"
#include "OnlineManager.h"

namespace basecross
{
namespace StageObject
{
	MatchStageReconnectUIObject::MatchStageReconnectUIObject(const std::shared_ptr<Stage>& stage) :
		UIObject(stage)
	{

	}

	std::shared_ptr<GameObject> MatchStageReconnectUIObject::CreateUIButtonObject(const std::shared_ptr<Stage>& stage, const std::wstring& spriteKey, float x, float y)
	{
		auto gameObject = GetThis<GameObject>();

		auto buttonObject = stage->AddGameObject<SimpleSprite>(SimpleSprite::Type::SpriteData, spriteKey);
		buttonObject->SetParent(gameObject);

		auto rectTransform = buttonObject->GetComponent<RectTransform>();
		rectTransform->SetPosition(x, y);

		auto selectAppearanceChanger = buttonObject->AddComponent<SelectAppearanceChanger>();
		//selectAppearanceChanger->SetDeselectedSize(Vec3(0.5f, 0.5f, 0.5f));
		selectAppearanceChanger->SetDeselectedColor(Col4(0.5f, 0.5f, 0.5f, 1.0f));

		auto button = buttonObject->AddComponent<Button>();

		std::weak_ptr<GameObject> weakThis = gameObject;

		// ボタンを押した際に自分を非表示にし、音を鳴らす
		button->AddPushEvent([weakThis]()
			{
				auto gameObject = weakThis.lock();

				if (!gameObject)
				{
					return;
				}

				gameObject->SetActive(false);

				SimpleSoundManager::OnePlaySE(L"DecisionSE");
			}
		);

		return buttonObject;
	}

	void MatchStageReconnectUIObject::OnCreate()
	{
		auto stage = GetStage();
		std::shared_ptr<RectTransform> rectTransform;

		auto cannotConnectSpriteObject = stage->AddGameObject<SimpleSprite>(SimpleSprite::Type::SpriteData, L"CannotConnect");
		cannotConnectSpriteObject->SetParent(GetThis<GameObject>());
		rectTransform = cannotConnectSpriteObject->GetRectTransform();
		rectTransform->SetPosition(0, 100);

		auto backTitleButtonObject = CreateUIButtonObject(stage, L"ToTitle", -400, -200);
		m_backTitleButtonObject = backTitleButtonObject;

		auto toContinueButtonObject = CreateUIButtonObject(stage, L"ToContinue", 400, -200);
		m_toContinueButtonObject = toContinueButtonObject;

		auto backTitleButton = backTitleButtonObject->GetComponent<Button>();
		auto toContinueButton = toContinueButtonObject->GetComponent<Button>();

		backTitleButton->SetRightSelectable(toContinueButton);
		toContinueButton->SetLeftSelectable(backTitleButton);
	}
}
}