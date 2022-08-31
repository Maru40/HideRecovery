/*!
@file   ScoreManager.h
@brief  各プレイヤーの情報を管理するシングルトンクラス
*/

#pragma once
#include "stdafx.h"
#include "../Utility/BaseSingleton.h"

namespace basecross {
	class ScoreManager :public BaseSingleton<ScoreManager> {
	public:
		struct Data {
			int KillCount;
			int GoalCount;
			Data() :KillCount(0), GoalCount(0) {}
		};

		/// <summary>
		/// キル数を追加
		/// </summary>
		/// <param name="playerNumber">追加するプレイヤー番号</param>
		void AddKillCount(int playerNumber);

		/// <summary>
		/// ゴール数を追加
		/// </summary>
		/// <param name="playerNumber">追加するプレイヤー番号</param>
		void AddGoalCount(int playerNumber);

		/// <summary>
		/// スコアデータの取得
		/// </summary>
		/// <param name="playerNumber">取得したいプレイヤーの番号</param>
		/// <returns>スコアデータ</returns>
		Data GetScoreData(int playerNumber);
	private:
		map<int, Data> m_playerScoreMap;
		void Init();
		// 以下シングルトン関係
		ScoreManager() { Init(); }
		~ScoreManager() {}
		friend class BaseSingleton<ScoreManager>;
	};
}
