#include "PasswordTextUI.h"

namespace basecross
{
namespace UI
{
	bool PasswordTextUI::TryPush(wchar_t wc)
	{
		// バックスペースが押されたら
		if (wc == VK_BACK)
		{
			if (!m_passwordText.empty())
			{
				m_passwordText.pop_back();
			}

			return false;
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
			return true;
		}

		return false;
	}

	PasswordTextUI::PasswordTextUI(const std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{
		itbs::Input::InputTextManager::AddInputText(this);
		m_passwordText.reserve(DEFAULT_PASSWORD_DIST_NUM);
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
		if (!TryPush(wc))
		{
			return false;
		}

		return true;
	}

	void PasswordTextUI::SetPasswordDisitNum(int passwordDisitNum)
	{
		m_passwordDisitNum = passwordDisitNum;

		if (m_passwordDisitNum > m_passwordText.capacity())
		{
			m_passwordText.reserve(passwordDisitNum);
		}
	}
}
}