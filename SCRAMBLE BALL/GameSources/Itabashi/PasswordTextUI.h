#pragma once
#include "Utility/InputTextManager.h"

namespace basecross
{
	class Numbers;

namespace UI
{
	/// <summary>
	/// キーボード入力のパスワードを表示するUIコンポーネント
	/// </summary>
	class PasswordTextUI : public SelectableComponent, public itbs::Input::I_InputText, public I_Submitable
	{
		const int m_passwordDisitNum = 4; // パスワードの桁数

		std::wstring m_passwordText; // パスワードの文字列データ
		std::weak_ptr<Numbers> m_numbers; // パスワード表示用Numbers

		int m_passwordNumber = 0; // パスワードの数字データ

		std::wstring m_decisionSEKey; // パスワードの決定音

		std::vector<std::function<void(const std::wstring&)>> m_submitEvents; // パスワード決定時イベント配列

		/// <summary>
		/// 入力を試す
		/// </summary>
		/// <param name="wc">入力文字</param>
		/// <returns>文字列に変更があればtrue</returns>
		bool TryPush(wchar_t wc);

	public:
		PasswordTextUI(const std::shared_ptr<GameObject>& owner);

		~PasswordTextUI() noexcept;

		void OnCreate() override;

		void OnUpdate() override;

		void OnDraw() override {}

		bool Push(wchar_t wc) override;

		const wchar_t* GetText() const override { return m_passwordText.c_str(); }

		void Clear() override;

		void OnSubmit() override;

		/// <summary>
		/// パスワード表示用Numbersの取得
		/// </summary>
		/// <param name="numbers">パスワード表示用Numbers</param>
		void SetNumbers(const std::shared_ptr<Numbers>& numbers) { m_numbers = numbers; }

		/// <summary>
		/// 入力が完了しているか
		/// </summary>
		/// <returns>完了しているならtrue</returns>
		bool IsComplete() const { return m_passwordText.size() >= m_passwordDisitNum; }

		/// <summary>
		/// パスワード決定時イベントの追加
		/// </summary>
		/// <param name="submitEvent">パスワード決定時イベント</param>
		void AddSubmitEvent(const std::function<void(const std::wstring&)>& submitEvent) { m_submitEvents.push_back(submitEvent); }

		/// <summary>
		/// パスワードの決定音の設定
		/// </summary>
		/// <param name="decisionSEKey">パスワードの決定音</param>
		void SetDecisionSEKey(const std::wstring& decisionSEKey) { m_decisionSEKey = decisionSEKey; }

		/// <summary>
		/// パスワードの決定音の取得
		/// </summary>
		/// <returns>パスワードの決定音</returns>
		const std::wstring& GetDecisionSEKey() const { return m_decisionSEKey; }
	};
}
}