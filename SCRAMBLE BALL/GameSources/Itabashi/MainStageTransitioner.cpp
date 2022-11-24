#include "stdafx.h"
#include "MainStageTransitioner.h"
#include "Watanabe/Manager/TimeManager.h"
#include "Scene.h"
#include "DisconnectToTitleUIObject.h"

namespace basecross
{
namespace Online
{
	MainStageTransitioner::MainStageTransitioner(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner),
		m_transitionDelayTime(1.0f)
	{

	}

	void MainStageTransitioner::OnLateStart()
	{
		auto disconnectToTileUIObject = m_disconnectToTitleUIObject.lock();

		if (!disconnectToTileUIObject)
		{
			return;
		}

		auto button = disconnectToTileUIObject->GetToTitleButtonObject()->GetComponent<Button>();
		std::weak_ptr<MainStageTransitioner> weakThis = GetThis<MainStageTransitioner>();

		button->AddPushEvent([weakThis]()
			{
				auto thisPtr = weakThis.lock();
				thisPtr->PostEvent(0.0f, thisPtr, App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
			}
		);
	}

	void MainStageTransitioner::OnUpdate()
	{
		if (!TimeManager::GetInstance()->GetTimer().IsTimeUp())
		{
			return;
		}

		m_elapsedTime += App::GetApp()->GetElapsedTime();

		if (m_elapsedTime < m_transitionDelayTime)
		{
			return;
		}

		PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToResultStage");
	}

	void MainStageTransitioner::OnDisconnected()
	{
		auto disconnectToTileUIObject = m_disconnectToTitleUIObject.lock();

		if (!disconnectToTileUIObject)
		{
			return;
		}

		disconnectToTileUIObject->SetActive(true);
		EventSystem::GetInstance(GetStage())->SetNowSelectableObject(disconnectToTileUIObject->GetToTitleButtonObject());
	}
}
}