#pragma once
#include "stdafx.h"
#include "Itabashi/OnlineManager.h"
#include "../UI/UIObjects.h"

namespace basecross {

	namespace StageObject
	{
		class MatchingSelectUIObject;
	}

	namespace Online
	{
		class OnlineMatching;
	}

	class MatchStageTransitioner;
	class PasscodeUI;

	/// <summary>
	/// MatchStageのUIコントロールコンポーネント
	/// </summary>
	class MatchingUIController :public Online::OnlineComponent {
		shared_ptr<UIObjectCSVBuilder> m_builder; // UIビルダー

		std::weak_ptr<MatchStageTransitioner> m_transitioner; // MatchStageの遷移用コンポーネント
		std::weak_ptr<Online::OnlineMatching> m_onlineMatching; // マッチング用コンポーネント

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

		/// <summary>
		/// マッチング選択用UIオブジェクトの設定
		/// </summary>
		/// <param name="selectUIObject">マッチング選択用UIオブジェクト</param>
		void SetMatchingSelectUIObject(const std::shared_ptr<StageObject::MatchingSelectUIObject>& selectUIObject) { m_selectUIObject = selectUIObject; }
		/// <summary>
		/// パスワード入力用UIオブジェクトの設定
		/// </summary>
		/// <param name="passcodeUIObject">パスワード入力用UIオブジェクト</param>
		void SetPasscodeUIObject(const std::shared_ptr<PasscodeUI>& passcodeUIObject) { m_passcodeUIObject = passcodeUIObject; }
		/// <summary>
		/// パスワード表示用UIオブジェクトの設定
		/// </summary>
		/// <param name="passwordViewNumbersObject">パスワード表示用UIオブジェクト</param>
		void SetPasswordViewNumbersObject(const std::shared_ptr<Numbers>& passwordViewNumbersObject) { m_passwordViewNumbersObject = passwordViewNumbersObject; }
		/// <summary>
		/// MatchStageの遷移用コンポーネントの設定
		/// </summary>
		/// <param name="transitioner">MatchStageの遷移用コンポーネント</param>
		void SetStageTransitioner(const std::shared_ptr<MatchStageTransitioner>& transitioner) { m_transitioner = transitioner; }
		/// <summary>
		/// マッチング用コンポーネントの設定
		/// </summary>
		/// <param name="onlineMatching">マッチング用コンポーネント</param>
		void SetOnlineMatching(const std::shared_ptr<Online::OnlineMatching>& onlineMatching) { m_onlineMatching = onlineMatching; }
	};
}
