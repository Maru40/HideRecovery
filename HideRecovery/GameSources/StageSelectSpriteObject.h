/*!
@file StageSelectSpriteObject.h
@brief StageSelectSpriteObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class SpriteObject;
	class Number;

	//--------------------------------------------------------------------------------------
	/// �X�e�[�W�Z���N�g�X�v���C�g�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class StageSelectSpriteObject : public GameObject
	{
	private:
		ex_weak_ptr<SpriteObject> m_unSelectBack;		//��I�����̃o�b�N�X�v���C�g
		ex_weak_ptr<SpriteObject> m_selectBack;			//�I�����̃o�b�N�X�v���C�g
		ex_weak_ptr<SpriteObject> m_main;				//���C���X�v���C�g
		ex_weak_ptr<SpriteObject> m_stage;				//�X�e�[�W�X�v���C�g
		ex_weak_ptr<Number> m_number;					//�i���o�[�X�v���C�g

		std::vector<ex_weak_ptr<GameObject>> m_objects; //���������S�ẴI�u�W�F�N�g�z��

		uint32_t m_num;          //�X�e�[�W�i���o�[
		wstring m_stageTexture;  //�X�e�[�W�e�N�X�`��

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		/// <param name="num">�X�e�[�W�i���o�[</param>
		/// <param name="stageTexture">�X�e�[�W�e�N�X�`��</param>
		StageSelectSpriteObject(const std::shared_ptr<Stage>& stage, const uint32_t num, const wstring& stageTexture);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// �o�b�N�X�v���C�g�̐���
		/// </summary>
		void CreateBack();

		/// <summary>
		/// ���C���X�v���C�g�̐���
		/// </summary>
		void CreateMain();

		/// <summary>
		/// �X�e�[�W�X�v���C�g�̐���
		/// </summary>
		void CreateStage();

		/// <summary>
		/// �i���o�[�X�v���C�g�̐���
		/// </summary>
		void CreateNum();
	};

}

//endbasecross