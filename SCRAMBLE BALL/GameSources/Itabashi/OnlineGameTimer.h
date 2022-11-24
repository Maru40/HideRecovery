#pragma once
#include "stdafx.h"
#include "OnlineManager.h"
#include <unordered_set>
#include <functional>

namespace basecross
{
	/// <summary>
	/// オンライン同期用ゲームタイマー
	/// </summary>
	class OnlineGameTimer : public Online::OnlineComponent
	{
	public:
		/// <summary>
		/// タイマー開始ができるかを問うオンラインイベントコード
		/// </summary>
		static constexpr std::uint8_t GAMETIMER_START_CHECK_EVENT_CODE = 200;
		/// <summary>
		/// タイマー開始可能を伝えるオンラインイベントコード
		/// </summary>
		static constexpr std::uint8_t GAMETIMER_START_OK_EVENT_CODE    = 201;
		/// <summary>
		/// ゲームタイマー開始を伝えるオンラインイベントコード
		/// </summary>
		static constexpr std::uint8_t GAMETIMER_START_EVENT_CODE       = 202;

	private:

		static constexpr int FINISH_COUNT_ELAPSED_TIME = 295;

		/// <summary>
		/// ゲームを開始できるか
		/// </summary>
		bool m_canGameStartable = false;
		/// <summary>
		/// ゲーム開始可能か
		/// </summary>
		bool m_isGameStartable = false;
		/// <summary>
		/// タイマーを更新するか
		/// </summary>
		bool m_isTimerUpdate = false;
		/// <summary>
		/// 
		/// </summary>
		bool m_isBeforeTimeUp = false;
		/// <summary>
		/// 
		/// </summary>
		bool m_isBeforeFinishCount = false;

		float m_timeCount = 0.0f;
		int m_count = 0;

		float m_startCheckSpan;

		std::unordered_set<int> m_startOkPlayerNumbers;

		std::vector<std::function<void()>> m_gameStartCountFuncs;

		std::vector<std::function<void()>> m_gameFinishCountEventFuncs;
		std::vector<std::function<void()>> m_gameFinishEventFuncs;

		/// <summary>
		/// ゲーム開始をチェックする
		/// </summary>
		void StartCheck();

		void GameCountStart();
		/// <summary>
		/// 強制的にタイムアップにする
		/// </summary>
		void TimeSkipToFinish();

	public:
		OnlineGameTimer(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnLateStart() override;

		void OnUpdate() override;

		void OnDisconnected() override;

		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;

		void OnLeaveRoomEventAction(int playerNumber, bool isInactive) override;

		/// <summary>
		/// ゲームスタート時のイベントを追加する
		/// </summary>
		/// <param name="gameStartCountFunc">ゲームスタート時のイベント</param>
		void AddGameStartCountFunc(const std::function<void()>& gameStartCountFunc) { m_gameStartCountFuncs.push_back(gameStartCountFunc); }
		/// <summary>
		/// ゲーム終了カウント時に呼ばれるイベントを追加
		/// </summary>
		/// <param name="gameFinishCountEventFunc">ゲーム終了カウント時に呼ばれるイベント</param>
		void AddGameFinishCountEventFunc(const std::function<void()>& gameFinishCountEventFunc) { m_gameFinishCountEventFuncs.push_back(gameFinishCountEventFunc); }
		/// <summary>
		/// ゲーム終了時に呼ばれるイベントを追加
		/// </summary>
		/// <param name="gameFinishEventFunc">ゲーム終了時に呼ばれるイベント</param>
		void AddGameFinishEventFunc(const std::function<void()>& gameFinishEventFunc) { m_gameFinishEventFuncs.push_back(gameFinishEventFunc); }

		/// <summary>
		/// ゲームのタイマーを開始する
		/// </summary>
		void GameTimerStart() { m_isTimerUpdate = true; }
		/// <summary>
		/// ゲームのタイマーを停止する
		/// </summary>
		void GameTimerStop() { m_isTimerUpdate = false; }
	};
}