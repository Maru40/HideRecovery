/*!
@file MainStage.h
@brief MainStage
*/

#pragma once
#include "stdafx.h"
#include "GameStageBase.h"
#include "Watanabe/UI/GameStartUI.h"
#include "Watanabe/UI/GameFinishUI.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// ���C���X�e�[�W
	//--------------------------------------------------------------------------------------
	class MainStage : public GameStageBase {
		static wstring sm_loadMapName;  //�ǂݍ��ރ}�b�v��

		//std::shared_ptr<GameStartUI> m_gameStartUI;
		//std::shared_ptr<GameFinishUI> m_gameFinishUI;

		/// <summary>
		/// �r���[�̍쐬
		/// </summary>
		void CreateViewLight();

	public:

		MainStage() :GameStageBase() {}
		virtual ~MainStage() {}

		virtual void OnCreate() override;
		virtual void OnUpdate() override;

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �ǂݍ��ރ}�b�v���̐ݒ�
		/// </summary>
		/// <param name="name">�ǂݍ��ރ}�b�v��</param>
		static void SetLoadMapName(const wstring& name);

		/// <summary>
		/// �ǂݍ��ރ}�b�v���̎擾
		/// </summary>
		/// <returns></returns>
		static wstring GetLoadMapName();

		//std::shared_ptr<GameStartUI> GetGameStartUI();
		//std::shared_ptr<GameFinishUI> GetGameFinishUI();
	};
}
//end basecross