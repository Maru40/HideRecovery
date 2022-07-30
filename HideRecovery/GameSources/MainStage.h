/*!
@file MainStage.h
@brief MainStage
*/

#pragma once
#include "stdafx.h"

#include "GameStageBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ���C���X�e�[�W
	//--------------------------------------------------------------------------------------
	class MainStage : public GameStageBase {
		static wstring sm_loadMapName;  //�ǂݍ��ރ}�b�v��

		/// <summary>
		/// �r���[�̍쐬
		/// </summary>
		void CreateViewLight();

	public:

		MainStage() :GameStageBase() {}
		virtual ~MainStage() {}

		virtual void OnCreate() override;

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

	};
}
//end basecross