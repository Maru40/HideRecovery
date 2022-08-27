/*!
@file   ScoreManager.cpp
@brief  各プレイヤーの情報を管理するシングルトンクラス実体
*/

#include "stdafx.h"
#include "ScoreManager.h"
#include "Itabashi/OnlineMatching.h"

namespace basecross {
	// インスタンスの初期化
	unique_ptr<ScoreManager, ScoreManager::Deleter> ScoreManager::m_instance = nullptr;

	void ScoreManager::Init() {
		for (int i = 0; i < Online::OnlineMatching::MAX_PLAYER_NUM; i++) {
			m_playerScoreMap[i] = Data();
		}
	}

	void ScoreManager::AddKillCount(int playerNumber) {
		m_playerScoreMap[playerNumber].KillCount++;
	}

	void ScoreManager::AddGoalCount(int playerNumber) {
		m_playerScoreMap[playerNumber].GoalCount++;
	}

	ScoreManager::Data ScoreManager::GetScoreData(int playerNumber) {
		return m_playerScoreMap[playerNumber];
	}
}