#pragma once
#include "stdafx.h"
#include "Itabashi/OnlineManager.h"
#include "../UI/UIObjects.h"

namespace basecross {

	namespace StageObject
	{
		class MatchingSelectUIObject;
	}

	class MatchStageTransitioner;
	class PasscodeUI;

	/// <summary>
	/// MatchStageのUIコントロールコンポーネント
	/// </summary>
	class MatchingUIController :public Online::OnlineComponent {
		shared_ptr<UIObjectCSVBuilder> m_builder; // UIビルダー

		std::weak_ptr<MatchStageTransitioner> m_transitioner; // MatchStageの遷移用コンポーネント

		std::weak_ptr<StageObject::MatchingSelectUIObject> m_selectUIObject; // マッチング選択用UIオブジェクト
		std::weak_ptr<PasscodeUI> m_passcodeUIObject; // パスワード入力用UIオブジェクト
		std::weak_ptr<Numbers> m_passwordViewNumbersObject; // パスワード表示用UIオブジェクト

		bool m_isJoinRoom = false; // 部屋に参加ならtrue

		/// <summary>
		/// UI同士のイベントのつながりの作成
		/// </summary>
		void CreateUIEvent();

	public:
		MatchingUIController(const shared_ptr<GameObject>& owner,
			const shared_ptr<UIObjectCSVBuilder>& builder);

		void OnLateStart() override;

		void OnUpdate() override;

		void OnConnected() override;

		// 部屋を作成
		void OnCreateRoom()override;
		// 部屋に参加
		void OnJoinRoom()override;

		void OnJoinRoomFailed(int errorCode) override;

		void OnLeaveRoom() override;

		/// <summary>
		/// マッチングの開始時に呼ぶUI表示処理
		/// </summary>
		void ChangeUIStartMatching();
		/// <summary>
		/// 部屋から出た際に呼ぶUI表示処理
		/// </summary>
		void ChangeUIStartLeaveRoom();

		void SetMatchingSelectUIObject(const std::shared_ptr<StageObject::MatchingSelectUIObject>& selectUIObject) { m_selectUIObject = selectUIObject; }
		void SetPasscodeUIObject(const std::shared_ptr<PasscodeUI>& passcodeUIObject) { m_passcodeUIObject = passcodeUIObject; }
		void SetPasswordViewNumbersObject(const std::shared_ptr<Numbers>& passwordViewNumbersObject) { m_passwordViewNumbersObject = passwordViewNumbersObject; }
		void SetStageTransitioner(const std::shared_ptr<MatchStageTransitioner>& transitioner) { m_transitioner = transitioner; }
	};
}
