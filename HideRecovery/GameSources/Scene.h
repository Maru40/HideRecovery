/*!
@file Scene.h
@brief �V�[��
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{
	public:
		float m_PlayerHP = 0.0f; // �v���C���[��HP���Ǘ�����
		float m_PlayerEnergy = 0.0f; // �v���C���[�̃G�l���M�[�ʂ��Ǘ�����

		int m_CountTime = 0; // �o�ߎ��Ԃ̃J�E���g�p
		int m_StageNumber = 0; // ���݂̃X�e�[�W�ԍ�

		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �C�x���g�擾
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;

		// -----------------------------�l���p���p�̊֐�--------------------------------
		// �v���C���[----------------------------------------------------------------------
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
		// �o�ߎ���---------------------
		void SetCountTime(int time)
		{
			m_CountTime = time;
		}

		int GetCountTime()
		{
			return m_CountTime;
		}
		// --------------------------------------------------------------------------------------
		// �X�e�[�W�ԍ�-------------------
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
