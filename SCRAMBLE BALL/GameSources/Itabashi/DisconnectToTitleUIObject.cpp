#include "DisconnectToTitleUIObject.h"
#include "Watanabe/UI/SimpleSprite.h"
#include "Watanabe/UI/InProcessUI.h"
#include "Itabashi/MainStageDisconnectUIController.h"
#include "Watanabe/UI/ButtonUI.h"

namespace basecross
{
namespace StageObject
{
	DisconnectToTitleUIObject::DisconnectToTitleUIObject(const std::shared_ptr<Stage>& stage) :
		UIObject(stage)
	{

	}

	void DisconnectToTitleUIObject::OnCreate()
	{
		auto stage = GetStage();
		auto thisObject = GetThis<GameObject>();
		auto& app = App::GetApp();

		std::shared_ptr<RectTransform> rectTransform;
		std::shared_ptr<Transform> transform;

		auto backGroundObject = stage->AddGameObject<UIObject>();
		backGroundObject->SetParent(thisObject);
		auto backGroundImage = backGroundObject->AddComponent<Image>();
		backGroundImage->SetTextureResource(L"SquareSprite_TX");
		backGroundImage->SetDiffuse(Col4(0.0f, 0.0f, 0.0f, 0.5f));
		rectTransform = backGroundObject->GetComponent<RectTransform>();
		rectTransform->SetRectSize(static_cast<float>(app->GetGameWidth()), static_cast<float>(app->GetGameHeight()));
		backGroundObject->SetDrawLayer(3);

		auto disconnectUIObject = stage->AddGameObject<UIObject>();
		disconnectUIObject->SetParent(thisObject);
		disconnectUIObject->SetActive(false);

		auto disconnectSpriteObject = stage->AddGameObject<SimpleSprite>();
		disconnectSpriteObject->SetParent(disconnectUIObject);
		disconnectSpriteObject->ChangeSprite(SimpleSprite::Type::SpriteData, L"CannotConnect");
		disconnectSpriteObject->SetDrawLayer(4);
		rectTransform = disconnectSpriteObject->GetComponent<RectTransform>();
		rectTransform->SetPosition(0, 200);

		auto buttonObject = stage->AddGameObject<UIObject>();
		buttonObject->SetParent(disconnectUIObject);
		buttonObject->AddComponent<Button>();
		m_toTitleButtonObject = buttonObject;

		auto buttonSpriteObject = stage->AddGameObject<ButtonUI>();
		buttonSpriteObject->SetParent(buttonObject);
		buttonSpriteObject->ChangeButton(L"B");
		buttonSpriteObject->SetChildDrawLayer(4);
		buttonSpriteObject->GetComponent<RectTransform>()->SetPosition(-260, -200);

		auto buttonTextObject = stage->AddGameObject<SimpleSprite>();
		buttonTextObject->SetParent(buttonObject);
		buttonTextObject->SetDrawLayer(4);
		buttonTextObject->ChangeSprite(SimpleSprite::Type::SpriteData, L"ToTitle");
		buttonTextObject->GetComponent<RectTransform>()->SetPosition(50, -200);

		auto reconnectUIObject = stage->AddGameObject<InProcessUI>();
		reconnectUIObject->SetParent(thisObject);
		reconnectUIObject->SetDrawLayer(4);
		reconnectUIObject->SetChildDrawLayer(4);
		reconnectUIObject->SetLabel(InProcessUI::LabelType::InReconnection);
		reconnectUIObject->SetActive(true);

		auto disconnectUIController = AddComponent<MainStageDisconnectUIController>();
		disconnectUIController->SetGroupUIObject(thisObject);
		disconnectUIController->SetWaitUIObject(reconnectUIObject);
		disconnectUIController->SetDisconnectUIObject(disconnectUIObject);
		disconnectUIController->SetSelectObject(buttonObject);
	}
}
}