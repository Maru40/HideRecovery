#pragma once

namespace basecross
{
	class UIObjectCSVBuilder;
	class PasscodeUI;
	class Numbers;

namespace StageObject
{
	class MatchingSelectUIObject;

	/// <summary>
	/// MatchStageのUIオブジェクト
	/// </summary>
	class MatchStageUIObject : public UIObject
	{
		std::shared_ptr<UIObjectCSVBuilder> m_builder; // UIビルダー

		std::weak_ptr<MatchingSelectUIObject> m_matchingSelectUIObject; // マッチング選択UIオブジェクト
		std::weak_ptr<PasscodeUI> m_passcodeUIObject; // パスワード入力用UIオブジェクト
		std::weak_ptr<Numbers> m_passwordViewNumbers; // パスワード表示用UIオブジェクト

	public:
		MatchStageUIObject(const std::shared_ptr<Stage>& stage, const std::shared_ptr<UIObjectCSVBuilder>& builder);

		void OnCreate();

		/// <summary>
		/// マッチング選択UIオブジェクトの取得
		/// </summary>
		/// <returns>マッチング選択UIオブジェクト</returns>
		std::shared_ptr<MatchingSelectUIObject> GetMatchingSelectUIObject() const { return m_matchingSelectUIObject.lock(); }
		/// <summary>
		/// パスワード入力用UIオブジェクトの取得
		/// </summary>
		/// <returns>パスワード入力用UIオブジェクト</returns>
		std::shared_ptr<PasscodeUI> GetPasscodeUIObject() const { return m_passcodeUIObject.lock(); }
		/// <summary>
		/// パスワード表示用UIオブジェクトの取得
		/// </summary>
		/// <returns>パスワード表示用UIオブジェクト</returns>
		std::shared_ptr<Numbers> GetPasswordViewNumbers() const { return m_passwordViewNumbers.lock(); }
	};
}
}