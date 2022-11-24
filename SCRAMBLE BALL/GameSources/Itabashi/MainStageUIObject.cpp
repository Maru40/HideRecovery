#include "MainStageUIObject.h"
#include "Watanabe/UI/GameStartUI.h"
#include "Watanabe/UI/GameFinishUI.h"
#include "Maruyama/Utility/SingletonComponent/GameManager.h"

namespace basecross
{
namespace StageObject
{
	MainStageUIObject::MainStageUIObject(const std::shared_ptr<Stage>& stage) :
		UIObject(stage)
	{

	}

	void MainStageUIObject::OnCreate()
	{
		auto stage = GetStage();
		auto thisObject = GetThis<GameObject>();

		auto gameStartUIObject = stage->AddGameObject<GameStartUI>();
		gameStartUIObject->SetParent(thisObject);
		m_gameStartUIObject = gameStartUIObject;

		auto gameFinishUIObject = stage->AddGameObject<GameFinishUI>();
		gameFinishUIObject->SetParent(thisObject);
		m_gameFinishUIObject = gameFinishUIObject;

		gameStartUIObject->AddTimeUpEventFunc([]() { SimpleSoundManager::ChangeBGM(L"GameStageBGM", 0.05f); });
		gameStartUIObject->AddTimeUpEventFunc([]() { SimpleSoundManager::OnePlaySE(L"GameStartSE", 0.25f); });

		//ゲーム状態にする
		gameStartUIObject->AddTimeUpEventFunc([]() { if (auto gameManager = GameManager::GetInstance()) { gameManager->ChangeState(GameManager::State::Game); } });

	}
}
}