#pragma once
#include "stdafx.h"
#include "OnlineManager.h"

namespace basecross
{
namespace Online
{
	class MainStageTransitioner : public OnlineComponent
	{
	public:

	private:

		float m_transitionDelayTime;
		float m_elapsedTime = 0.0f;

	public:
		MainStageTransitioner(const std::shared_ptr<GameObject>& owner);

		void OnUpdate() override;

		void SetTransitionDelayTime(float transitionDelayTime) { m_transitionDelayTime = transitionDelayTime; }
		float GetTransitionDelayTime() const { return m_transitionDelayTime; }
	};
}
}