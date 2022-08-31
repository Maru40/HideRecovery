/*!
@file Scene.h
@brief シーン
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{
	public:
		float m_PlayerHP = 0.0f; // プレイヤーのHPを管理する
		float m_PlayerEnergy = 0.0f; // プレイヤーのエネルギー量を管理する

		int m_CountTime = 0; // 経過時間のカウント用
		int m_StageNumber = 0; // 現在のステージ番号

		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief イベント取得
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;

		// -----------------------------値引継ぎ用の関数--------------------------------
		// プレイヤー----------------------------------------------------------------------
		void SetPlayerHP(float HP)
		{
			m_PlayerHP = HP;
		}

		float GetPlayerHP()
		{
			return m_PlayerHP;
		}

		void SetPlayerEnergy(float EN)
		{
			m_PlayerEnergy = EN;
		}

		float GetPlayerEnergy()
		{
			return m_PlayerEnergy;
		}

		// --------------------------------------------------------------------------------------
		// 経過時間---------------------
		void SetCountTime(int time)
		{
			m_CountTime = time;
		}

		int GetCountTime()
		{
			return m_CountTime;
		}
		// --------------------------------------------------------------------------------------
		// ステージ番号-------------------
		void SetStageName(wstring StageName)
		{
			if (StageName == L"StageS1.csv")
			{
				m_StageNumber = 0;
			}
			else if(StageName == L"StageS2.csv")
			{
				m_StageNumber = 1;
			}
			else if (StageName == L"StageM1.csv")
			{
				m_StageNumber = 2;
			}
			else if (StageName == L"StageM2.csv")
			{
				m_StageNumber = 3;
			}
			else if (StageName == L"StageL1.csv")
			{
				m_StageNumber = 4;
			}
			else if (StageName == L"StageL2.csv")
			{
				m_StageNumber = 5;
			}
/*			else if (StageName == L"StageA.csv")
			{
				m_StageNumber = 6;
			}	*/			
		}

		int GetStageName()
		{
			return m_StageNumber;
		}
	};

}

//end basecross
