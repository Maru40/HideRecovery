#pragma once
#include "stdafx.h"
#include "../Utility/BaseSingleton.h"
#include "../StageObject/PlayerSpawnPointObject.h"

namespace basecross {
	class PointManager :public BaseSingleton<PointManager> {
		// それぞれのチームのスコア
		unordered_map<team::TeamType, int> m_teamScoreMap;
	public:
		enum class WinOrLose {
			Win,
			Draw,
			Lose
		};

		/// <summary>
		/// 点数を取得
		/// </summary>
		/// <param name="team">どのチームのポイントか</param>
		/// <returns>そのチームの点数</returns>
		int GetPoint(team::TeamType team);

		/// <summary>
		/// 点数を1点追加
		/// </summary>
		void AddPoint(team::TeamType team);

		/// <summary>
		/// 自チームの勝敗を取得する
		/// </summary>
		/// <param name="team">チーム</param>
		/// <returns>勝敗</returns>
		WinOrLose GetSelfTeamWinOrLose(team::TeamType team);

		/// <summary>
		/// 引き分けか
		/// </summary>
		/// <returns>trueなら引き分け</returns>
		bool IsDraw();

		/// <summary>
		/// 勝者を取得
		/// </summary>
		/// <returns>勝者</returns>
		team::TeamType GetWinner();
	private:
		PointManager();
		~PointManager() {}
		friend class BaseSingleton<PointManager>;
	};
}
