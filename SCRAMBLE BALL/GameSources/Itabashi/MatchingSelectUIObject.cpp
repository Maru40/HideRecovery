#include "MatchingSelectUIObject.h"
#include "Watanabe/UI/SimpleSprite.h"
#include "SelectAppearanceChanger.h"
#include "UICancelEventer.h"
#include "OnlineManager.h"

namespace basecross
{
namespace StageObject
{
	MatchingSelectUIObject::MatchingSelectUIObject(const std::shared_ptr<Stage>& stage) :
		UIObject(stage)
	{

	}

	std::shared_ptr<GameObject> MatchingSelectUIObject::CreateUIButtonObject(const std::shared_ptr<Stage>& stage, const std::wstring& spriteKey, float x, float y)
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

		auto cancelEventer = buttonObject->AddComponent<UICancelEventer>();
		std::weak_ptr<Stage> weakStage = GetStage();

		// ボタン上でキャンセルが押されたら、接続を切り操作を無効にする
		cancelEventer->AddCancelEvent([weakStage]()
			{
				EventSystem::GetInstance(weakStage.lock())->PushSelectableObject(nullptr);
			}
		);

		return buttonObject;
	}

	void MatchingSelectUIObject::OnCreate()
	{
		auto stage = GetStage();

		auto freeMatchingButtonObject = CreateUIButtonObject(stage, L"FreeMatch", 0, 200);
		m_freeMatchingButtonObject = freeMatchingButtonObject;

		auto createRoomButtonObject = CreateUIButtonObject(stage, L"CreateRoomLabel", 0, 0);
		m_createRoomButtonObject = createRoomButtonObject;

		auto joinRoomButtonObject = CreateUIButtonObject(stage, L"JoinRoomLabel", 0, -200);
		m_joinRoomButtonObject = joinRoomButtonObject;

		// ボタンの選択移動の設定

		auto freeMatchingButton = freeMatchingButtonObject->GetComponent<Button>();
		auto createRoomButton = createRoomButtonObject->GetComponent<Button>();
		auto joinRoomButton = joinRoomButtonObject->GetComponent<Button>();

		freeMatchingButton->SetDownSelectable(createRoomButton);
		createRoomButton->SetUpSelectable(freeMatchingButton);
		createRoomButton->SetDownSelectable(joinRoomButton);
		joinRoomButton->SetUpSelectable(createRoomButton);
	}
}
}