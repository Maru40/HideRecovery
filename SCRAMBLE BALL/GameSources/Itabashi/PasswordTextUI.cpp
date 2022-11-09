#include "PasswordTextUI.h"
#include "Watanabe/UI/Numbers.h"

namespace basecross
{
namespace UI
{
	bool PasswordTextUI::TryPush(wchar_t wc)
	{
		if (!IsSelected())
		{
			return false;
		}
		
		// バックスペースが押されたら
		if (wc == VK_BACK)
		{
			if (!m_passwordText.empty())
			{
				m_passwordText.pop_back();
				
				m_passwordNumber /= 10;
			}

			return true;
		}

		// パスワード桁数に達していたら
		if (IsComplete())
		{
			return false;
		}


		// Enterは無視する
		if (wc == VK_RETURN)
		{
			return false;
		}



		if (L'0' <= wc && wc <= L'9')
		{
			m_passwordText.push_back(wc);

			m_passwordNumber *= 10;
			m_passwordNumber += wc - L'0';

			return true;
		}

		return false;
	}

	PasswordTextUI::PasswordTextUI(const std::shared_ptr<GameObject>& owner) :
		SelectableComponent(owner)
	{
		itbs::Input::InputTextManager::AddInputText(this);
		m_passwordText.reserve(m_passwordDisitNum);
	}

	PasswordTextUI::~PasswordTextUI()
	{
		itbs::Input::InputTextManager::RemoveInputText(this);
	}

	void PasswordTextUI::OnCreate()
	{

	}

	void PasswordTextUI::OnUpdate()
	{

	}

	bool PasswordTextUI::Push(wchar_t wc)
	{
		// 入力に変更がなければ
		if (!TryPush(wc))
		{
			return false;
		}

		auto numbers = m_numbers.lock();

		if (!numbers)
		{
			return true;
		}

		int outputNumber = m_passwordNumber * static_cast<int>(std::pow(10, m_passwordDisitNum - m_passwordText.size()));

		numbers->SetNumber(outputNumber);

		return true;
	}

	void PasswordTextUI::Clear()
	{
		m_passwordText.clear();
		m_passwordNumber = 0;

		auto numbers = m_numbers.lock();

		if (numbers)
		{
			numbers->SetNumber(0);
		}
	}

	void PasswordTextUI::OnSubmit()
	{
		if (!IsComplete())
		{
			return;
		}

		for (const auto& submitEvent : m_submitEvents)
		{
			submitEvent(m_passwordText);
			SimpleSoundManager::OnePlaySE(m_decisionSEKey);
		}
	}
}
}