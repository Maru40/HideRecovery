#pragma once

namespace basecross
{
namespace StageObject
{
	class MatchStageReconnectUIObject : public UIObject
	{
		std::weak_ptr<GameObject> m_backTitleButtonObject;
		std::weak_ptr<GameObject> m_toContinueButtonObject;

		std::shared_ptr<GameObject> CreateUIButtonObject(const std::shared_ptr<Stage>& stage, const std::wstring& spriteKey, float x, float y);
	public:
		MatchStageReconnectUIObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

		std::shared_ptr<GameObject> GetBackTitleButtonObject() const { return m_backTitleButtonObject.lock(); }
		std::shared_ptr<GameObject> GetToContinueButtonObject() const { return m_toContinueButtonObject.lock(); }
	};
}
}