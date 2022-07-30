#include"MessageWindow.h"

namespace basecross
{
	// MessageWindow ---------------------------

	MessageWindow::MessageWindow(std::shared_ptr<GameObject>& owner) :
		SelectableComponent(owner)
	{

	}

	int MessageWindow::CreateAddCharCount(const int addCharCount)
	{
		for (int i = 0; i < addCharCount; i++)
		{
			auto c = m_messageText[m_textEndCharCount + i + 1];
			if (c == L'\n' || c == L'\0')
			{
				return i + 1;
			}
		}

		return addCharCount;
	}

	void MessageWindow::UpdateMessage()
	{
		m_textBox->SetText(m_messageText.substr(m_textFrontCharCount, m_textEndCharCount - m_textFrontCharCount));
	}

	void MessageWindow::SetMessageText(const std::wstring& messageText)
	{
		m_textBox->SetText(L"");
		m_messageText = messageText;
		m_isPlaying = false;
		m_textFrontCharCount = 0;
		m_textEndCharCount = 0;
		m_isFinish = false;
		m_nowTime = 0.0f;
	}

	void MessageWindow::SetOneSecondDisplayedCharNum(const float oneSecondDisplayedCharNum)
	{
		m_oneSecondDisplayedCharNum = oneSecondDisplayedCharNum;
	}

	void MessageWindow::PlayMessage()
	{
		m_isPlaying = true;
	}

	void MessageWindow::OnStart()
	{
		m_textBox = GetGameObject()->GetComponent<TextBox>();
	}

	void MessageWindow::OnUpdate()
	{
		if (m_isFinish || !m_isPlaying)
		{
			return;
		}

		m_nowTime += App::GetApp()->GetElapsedTime();

		float checkTime = 1.0f / m_oneSecondDisplayedCharNum;

		if (m_nowTime < checkTime)
		{
			return;
		}

		int addCharCount = static_cast<int>(m_nowTime / checkTime);

		addCharCount = CreateAddCharCount(addCharCount);

		m_nowTime -= addCharCount * checkTime;
		m_textEndCharCount += addCharCount;

		UpdateMessage();

		CheckMessageFlag();
	}

	void MessageWindow::OnSelect()
	{

	}

	void MessageWindow::OnPush()
	{
		if (m_isFinish)
		{
			finishPushEvent();
			return;
		}

		if (m_isPlaying)
		{
			m_textEndCharCount += CreateAddCharCount((int)m_messageText.length() - m_textEndCharCount);
			UpdateMessage();
			CheckMessageFlag();
		}
		else
		{
			m_textFrontCharCount = m_textEndCharCount + 1;
			m_textEndCharCount++;

			m_textBox->SetText(L"");
			m_isPlaying = true;
		}
	}

	void MessageWindow::OnOutSelect()
	{

	}

	void MessageWindow::CheckMessageFlag()
	{
		auto endChar = m_messageText[m_textEndCharCount];
		if (endChar == L'\0')
		{
			m_isFinish = true;
			return;
		}

		if (endChar == L'\n')
		{
			m_isPlaying = false;
			return;
		}
	}

	// MessageWindowObject --------------------
	
	MessageWindowObject::MessageWindowObject(std::shared_ptr<Stage>& stage) :
		UIObject(stage)
	{

	}

	void MessageWindowObject::OnCreate()
	{
		AddComponent<TextBox>();
		AddComponent<MessageWindow>();
	}
}