
/*!
@file GraphNodeBase.h
@brief GraphNodeBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���t�m�[�h�̋K��N���X
	//--------------------------------------------------------------------------------------
	class GraphNodeBase
	{
		bool m_isActive; //�A�N�e�B�u�ȏ�Ԃ��ǂ���
		int m_index;     //�m�[�h�C���f�b�N�X

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		GraphNodeBase();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="index">���̃N���X�̃C���f�b�N�X</param>
		GraphNodeBase(const int& index);

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �C���f�b�N�X�̐ݒ�
		/// </summary>
		/// <param name="index">�C���f�b�N�X</param>
		void SetIndex(const int& index) noexcept { m_index = index; }

		/// <summary>
		/// �C���f�b�N�X�̎擾
		/// </summary>
		/// <returns>�C���f�b�N�X</returns>
		int GetIndex() const noexcept { return m_index; }

		/// <summary>
		/// �A�N�e�B�u��Ԃ�ݒ�
		/// </summary>
		/// <param name="isActive">�A�N�e�B�u���</param>
		void SetIsActive(const bool isActive) noexcept { m_isActive = isActive; }

		/// <summary>
		/// �A�N�e�B�u��Ԃ��擾
		/// </summary>
		/// <returns>�A�N�e�B�u��Ԃ��ǂ���</returns>
		bool IsActive() const noexcept { return m_isActive; }

	};

}