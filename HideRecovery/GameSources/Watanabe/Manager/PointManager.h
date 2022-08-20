#pragma once
#include "stdafx.h"
#include "../Utility/BaseSingleton.h"
#include "../StageObject/PlayerSpawnPointObject.h"

namespace basecross {
	class PointManager :public BaseSingleton<PointManager> {
		// それぞれのチームのスコア
		unordered_map<Team, int> m_teamScoreMap;
	public:
		/// <summary>
		/// 点数を取得
		/// </summary>
		/// <param name="team">どのチームのポイントか</param>
		/// <returns>そのチームの点数</returns>
		int GetPoint(Team team);

		/// <summary>
		/// 点数を1点追加
		/// </summary>
		void AddPoint(Team team);
	private:
		PointManager();
		~PointManager() {}
		friend class BaseSingleton<PointManager>;
	};
}
