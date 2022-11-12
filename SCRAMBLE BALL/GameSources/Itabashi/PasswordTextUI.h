#pragma once

namespace basecross
{
	class Numbers;

namespace UI
{
	/// <summary>
	/// キーボード入力のパスワードを表示するUIコンポーネント
	/// </summary>
	class PasswordTextUI : public SelectableComponent, public I_Submitable
	{
		const int m_passwordDisitNum = 4; // パスワードの桁数

		std::wstring m_passwordText; // パスワードの文字列データ
		std::weak_ptr<Numbers> m_numbers; // パスワード表示用Numbers

		std::weak_ptr<GameObject> m_upTriangle; // 上矢印オブジェクト
		std::weak_ptr<GameObject> m_downTriangle; // 下矢印オブジェクト

		int m_targetPasswordDisitNum = 0; // パスワードの現在の選択桁

		std::wstring m_decisionSEKey; // パスワードの決定音

		std::vector<std::function<void(const std::wstring&)>> m_submitEvents; // パスワード決定時イベント配列

		/// <summary>
		/// 現在の選択桁を更新する
		/// </summary>
		/// <param name="direction">入力方向</param>
		void UpdateTrianglePosition(UIMoveDirection direction);

		/// <summary>
		/// 数字を変更する
		/// </summary>
		/// <param name="direction">入力方向</param>
		void ChangeNumber(UIMoveDirection direction);

	public:
		PasswordTextUI(const std::shared_ptr<GameObject>& owner);

		void OnLateStart() override;

		void OnDraw() override {}

		void OnSubmit() override;

		void OnMove(UIMoveDirection direction) override;

		/// <summary>
		/// 入力パスワードの取得
		/// </summary>
		/// <returns>入力パスワード</returns>
		const std::wstring& GetPassword() const { return m_passwordText; }

		/// <summary>
		/// パスワードの初期化
		/// </summary>
		void Clear();

		/// <summary>
		/// パスワード表示用Numbersの設定
		/// </summary>
		/// <param name="numbers">パスワード表示用Numbers</param>
		void SetNumbers(const std::shared_ptr<Numbers>& numbers) { m_numbers = numbers; }
		/// <summary>
		/// 上矢印オブジェクトを設定
		/// </summary>
		/// <param name="upTriangle">上矢印オブジェクト</param>
		void SetUpTriangle(const std::shared_ptr<GameObject>& upTriangle) { m_upTriangle = upTriangle; }
		/// <summary>
		/// 下矢印オブジェクトの設定
		/// </summary>
		/// <param name="downTriangle">下矢印オブジェクト</param>
		void SetDownTriangle(const std::shared_ptr<GameObject>& downTriangle) { m_downTriangle = downTriangle; }

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