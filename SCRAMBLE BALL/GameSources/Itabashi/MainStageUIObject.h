#pragma once

namespace basecross
{
	class GameStartUI;
	class GameFinishUI;

namespace StageObject
{
	class DisconnectToTitleUIObject;

	class MainStageUIObject : public UIObject
	{
		std::weak_ptr<GameStartUI> m_gameStartUIObject;
		std::weak_ptr<GameFinishUI> m_gameFinishUIObject;
		std::weak_ptr<DisconnectToTitleUIObject> m_disconnectToTitleUIObject;

	public:
		MainStageUIObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

		std::shared_ptr<GameStartUI> GetGameStartUI() const noexcept { return m_gameStartUIObject.lock(); }

		std::shared_ptr<GameFinishUI> GetGameFinishUI() const noexcept { return m_gameFinishUIObject.lock(); }

		std::shared_ptr<DisconnectToTitleUIObject> GetDisconnectToTitleUIObject() const noexcept { return m_disconnectToTitleUIObject.lock(); }
	};
}
}