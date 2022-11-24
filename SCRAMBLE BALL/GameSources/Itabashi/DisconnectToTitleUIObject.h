#pragma once

namespace basecross
{
namespace StageObject
{
	class DisconnectToTitleUIObject : public UIObject
	{
		std::weak_ptr<GameObject> m_toTitleButtonObject;

	public:
		DisconnectToTitleUIObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

		std::shared_ptr<GameObject> GetToTitleButtonObject() const { return m_toTitleButtonObject.lock(); }
	};
}
}