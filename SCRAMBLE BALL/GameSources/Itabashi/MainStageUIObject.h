#pragma once

namespace basecross
{
	class GameStartUI;
	class GameFinishUI;

namespace StageObject
{
	class MainStageUIObject : public UIObject
	{
		std::weak_ptr<GameStartUI> m_gameStartUIObject;
		std::weak_ptr<GameFinishUI> m_gameFinishUIObject;

	public:
		MainStageUIObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

		std::shared_ptr<GameStartUI> GetGameStartUI() const { return m_gameStartUIObject.lock(); }

		std::shared_ptr<GameFinishUI> GetGameFinishUI() const { return m_gameFinishUIObject.lock(); }
	};
}
}