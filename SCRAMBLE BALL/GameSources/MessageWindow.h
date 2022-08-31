#pragma once

namespace basecross
{
	class MessageWindow : public SelectableComponent
	{
		ex_weak_ptr<TextBox> m_textBox;

		std::wstring m_messageText;

		int m_textFrontCharCount = 0;
		int m_textEndCharCount = 0;

		float m_nowTime = 0.0f;
		float m_oneSecondDisplayedCharNum = 10;

		bool m_isPlaying = false;

		bool m_isFinish = true;

		int CreateAddCharCount(const int addCharCount);

		void UpdateMessage();
	public:
		itbs::Utility::Delegate<void()> finishPushEvent;

		MessageWindow(std::shared_ptr<GameObject>& owner);

		void SetMessageText(const std::wstring& messageText);

		void SetOneSecondDisplayedCharNum(const float oneSecondDisplayedCharNum);

		void PlayMessage();

		void OnStart() override;

		void OnUpdate() override;

		void OnSelect() override;

		void OnPush() override;

		void OnOutSelect() override;

		void CheckMessageFlag();
	};

	class MessageWindowObject : public UIObject
	{
	public:
		MessageWindowObject(std::shared_ptr<Stage>& stage);

		void OnCreate() override;
	};
}