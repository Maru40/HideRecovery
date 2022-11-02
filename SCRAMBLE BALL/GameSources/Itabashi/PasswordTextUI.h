#pragma once
#include "Utility/InputTextManager.h"

namespace basecross
{
namespace UI
{
	class PasswordTextUI : public Component, public itbs::Input::I_InputText
	{
		static constexpr int DEFAULT_PASSWORD_DIST_NUM = 5;

		std::wstring m_passwordText;
		int m_passwordDisitNum = DEFAULT_PASSWORD_DIST_NUM;

		bool TryPush(wchar_t wc);

	public:
		PasswordTextUI(const std::shared_ptr<GameObject>& owner);

		~PasswordTextUI() noexcept;

		void OnCreate() override;

		void OnUpdate() override;

		void OnDraw() override {}

		bool Push(wchar_t wc) override;

		const wchar_t* GetText() const override { return m_passwordText.c_str(); }

		void Clear() override { m_passwordText.clear(); }

		void SetPasswordDisitNum(int passwordDisitNum);
		int GetPasswordDisitNum() const { return m_passwordDisitNum; }

		bool IsComplete() const { return m_passwordText.size() >= m_passwordDisitNum; }
	};
}
}