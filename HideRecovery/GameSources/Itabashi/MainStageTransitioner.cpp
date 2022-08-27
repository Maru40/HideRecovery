#include "stdafx.h"
#include "MainStageTransitioner.h"
#include "Watanabe/Manager/TimeManager.h"
#include "Scene.h"

namespace basecross
{
namespace Online
{
	MainStageTransitioner::MainStageTransitioner(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner),
		m_transitionDelayTime(1.0f)
	{

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
}
}