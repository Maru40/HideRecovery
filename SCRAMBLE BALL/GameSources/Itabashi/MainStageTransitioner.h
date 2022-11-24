#pragma once
#include "stdafx.h"
#include "OnlineManager.h"

namespace basecross
{
	namespace StageObject
	{
		class DisconnectToTitleUIObject;
	}

namespace Online
{
	class MainStageTransitioner : public OnlineComponent
	{
		std::weak_ptr<StageObject::DisconnectToTitleUIObject> m_disconnectToTitleUIObject;

		float m_transitionDelayTime;
		float m_elapsedTime = 0.0f;

	public:
		MainStageTransitioner(const std::shared_ptr<GameObject>& owner);

		void OnLateStart() override;

		void OnUpdate() override;

		void OnDisconnected() override;

		void SetTransitionDelayTime(float transitionDelayTime) { m_transitionDelayTime = transitionDelayTime; }
		float GetTransitionDelayTime() const { return m_transitionDelayTime; }

		void SetDisconnectToTitleUIObject(const std::shared_ptr<StageObject::DisconnectToTitleUIObject>& disconnectToTitleUIObject) { m_disconnectToTitleUIObject = disconnectToTitleUIObject; }
	};
}
}