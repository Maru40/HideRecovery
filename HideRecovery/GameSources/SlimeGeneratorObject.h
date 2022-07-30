
/*!
@file SlimeGeneratorObject.h
@brief SlimeGeneratorObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �X���C�������I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class SlimeGeneratorObject : public GameObject
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		SlimeGeneratorObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

	private:
		/// <summary>
		/// �ʏ�X���C���̐���
		/// </summary>
		void CreateNormalSlimeGenerator();

		/// <summary>
		/// �V���b�g�X���C���̐���
		/// </summary>
		void CreateShootSlimeGenerator();

		/// <summary>
		/// ������X���C���̐���
		/// </summary>
		void CreateEscapeSlimeGenerator();

		/// <summary>
		/// ����X���C���̐���
		/// </summary>
		void CreateBigSlimeGenerator();

		/// <summary>
		/// �t�@�C�����琶���ꏊ���擾���A���ꂼ��̃X���C���𐶐�����B
		/// </summary>
		void CreateLoadPostitionGenerator();

	};

}

//endbasecross