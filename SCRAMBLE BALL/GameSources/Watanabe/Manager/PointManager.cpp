#include "stdafx.h"
#include "PointManager.h"

namespace basecross {
	// インスタンスの初期化
	unique_ptr<PointManager, PointManager::Deleter> PointManager::m_instance = nullptr;

	PointManager::PointManager() {
		// 点数の初期化
		m_teamScoreMap[team::TeamType::Blue] = 0;
		m_teamScoreMap[team::TeamType::Red] = 0;
	}

	int PointManager::GetPoint(team::TeamType team) {
		return m_teamScoreMap[team];
	}
	void PointManager::AddPoint(team::TeamType team) {
		m_teamScoreMap[team]++;
	}

	PointManager::WinOrLose PointManager::GetSelfTeamWinOrLose(team::TeamType team) {
		int eastPoint = m_teamScoreMap[team::TeamType::Blue];
		int westPoint = m_teamScoreMap[team::TeamType::Red];
		if (eastPoint == westPoint) {
			return WinOrLose::Draw;
		}

		// 双方の点数が異なれば勝敗は決まる
		switch (team)
		{
		case team::TeamType::Blue:
			if (eastPoint > westPoint) {
				return WinOrLose::Win;
			}
			else {
				return WinOrLose::Lose;
			}
			break;
		case team::TeamType::Red:
			if (eastPoint < westPoint) {
				return WinOrLose::Win;
			}
			else {
				return WinOrLose::Lose;
			}
			break;
		}

		// ここまで実行されないが一応
		return WinOrLose::Draw;
	}

	bool PointManager::IsDraw() {
		int bluePoint = m_teamScoreMap[team::TeamType::Blue];
		int redPoint = m_teamScoreMap[team::TeamType::Red];
		return bluePoint == redPoint;
	}
	team::TeamType PointManager::GetWinner() {
		int bluePoint = m_teamScoreMap[team::TeamType::Blue];
		int redPoint = m_teamScoreMap[team::TeamType::Red];
		if (bluePoint < redPoint) {
			return team::TeamType::Red;
		}
		else {
			return team::TeamType::Blue;
		}
	}
}