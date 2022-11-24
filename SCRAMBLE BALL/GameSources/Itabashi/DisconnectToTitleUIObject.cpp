#include "DisconnectToTitleUIObject.h"
#include "Watanabe/UI/SimpleSprite.h"

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

		auto disconnectSpriteObject = stage->AddGameObject<SimpleSprite>();
		disconnectSpriteObject->SetParent(thisObject);
		disconnectSpriteObject->ChangeSprite(SimpleSprite::Type::SpriteData, L"CannotConnect");
		disconnectSpriteObject->SetDrawLayer(4);
		rectTransform = disconnectSpriteObject->GetComponent<RectTransform>();
		rectTransform->SetPosition(0, 200);

		auto toTitleButtonObject = stage->AddGameObject<SimpleSprite>();
		toTitleButtonObject->SetParent(thisObject);
		m_toTitleButtonObject = toTitleButtonObject;
		toTitleButtonObject->ChangeSprite(SimpleSprite::Type::SpriteData, L"ToTitle");
		toTitleButtonObject->SetDrawLayer(4);
		rectTransform = toTitleButtonObject->GetComponent<RectTransform>();
		rectTransform->SetPosition(0, -200);

		auto button = toTitleButtonObject->AddComponent<Button>();
	}
}
}