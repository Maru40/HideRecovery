#pragma once

namespace basecross
{
namespace StageObject
{
	/// <summary>
	/// マッチング選択を持ったオブジェクト
	/// </summary>
	class MatchingSelectUIObject : public UIObject
	{
		std::weak_ptr<GameObject> m_freeMatchingButtonObject; // フリーマッチングボタンオブジェクト
		std::weak_ptr<GameObject> m_createRoomButtonObject; // 部屋を作成するボタンオブジェクト
		std::weak_ptr<GameObject> m_joinRoomButtonObject; // 部屋に参加するボタンオブジェクト

		/// <summary>
		/// UIで使用するボタンの設定をして返す関数
		/// </summary>
		/// <param name="stage">ステージポインタ</param>
		/// <param name="spriteKey">スプライトのキー</param>
		/// <param name="x">x座標</param>
		/// <param name="y">y座標</param>
		/// <returns>ボタンオブジェクト</returns>
		std::shared_ptr<GameObject> CreateUIButtonObject(const std::shared_ptr<Stage>& stage, const std::wstring& spriteKey, float x, float y);

	public:

		MatchingSelectUIObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

		/// <summary>
		/// フリーマッチングボタンオブジェクトの取得
		/// </summary>
		/// <returns>フリーマッチングボタンオブジェクト</returns>
		std::shared_ptr<GameObject> GetFreeMatchingButtonObject() const { return m_freeMatchingButtonObject.lock(); }
		/// <summary>
		/// 部屋を作成するボタンオブジェクトの取得
		/// </summary>
		/// <returns>部屋を作成するボタンオブジェクト</returns>
		std::shared_ptr<GameObject> GetCreateRoomButtonObject() const { return m_createRoomButtonObject.lock(); }
		/// <summary>
		/// 部屋に参加するボタンオブジェクトの取得
		/// </summary>
		/// <returns>部屋に参加するボタンオブジェクト</returns>
		std::shared_ptr<GameObject> GetJoinRoomButtonObject() const { return m_joinRoomButtonObject.lock(); }
	};
}
}