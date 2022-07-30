#include "stdafx.h"
#include "satoComponent.h"

namespace basecross
{
	void FallisDestroy::OnCreate()
	{

	}

	void FallisDestroy::OnUpdate()
	{
		auto transformComponent = GetGameObject()->GetComponent<Transform>();
		m_ObjectPosition = transformComponent->GetPosition();

		auto stage = GetStage();
		if (stage)
		{
			auto stageObj = stage->GetGameObjectVec();
			for (auto obj : stageObj)
			{
				if (obj)
				{
					if (m_ObjectPosition.y < m_DesPos)
					{
						auto transComponent = GetGameObject()->GetComponent<Transform>();
						m_ObjectPosition = transComponent->GetPosition();
						obj->DestroyGameObject();
					}
				}
			}
		}
	}
}