#pragma once

namespace basecross
{
	class UICancelEventer : public Component, public I_Cancelable
	{
		std::vector<std::function<void()>> m_cancelEvents;
	public:
		UICancelEventer(const std::shared_ptr<GameObject>& owner);

		void OnCancel() override;

		void AddCancelEvent(const std::function<void()>& cancelEvent) { m_cancelEvents.push_back(cancelEvent); }
	};
}