/*!
@file GraphEdgeBase.h
@brief GraphEdgeBase
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�G�b�W�̊��N���X
	//--------------------------------------------------------------------------------------
	class GraphEdgeBase
	{
		int m_from;   //��O�̃m�[�h�C���f�b�N�X
		int m_to;     //��̃m�[�h�C���f�b�N�X

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="from">��O�̃m�[�h�C���f�b�N�X</param>
		/// <param name="to">��̃m�[�h�C���f�b�N�X</param>
		GraphEdgeBase(const int& from, const int& to)
			:m_from(from), m_to(to)
		{}

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ��O�̃m�[�h�C���f�b�N�X�̎擾
		/// </summary>
		/// <returns>��O�̃m�[�h�C���f�b�N�X</returns>
		int GetFrom() const noexcept { return m_from; }

		/// <summary>
		/// ��̃m�[�h�C���f�b�N�X�̎擾
		/// </summary>
		/// <returns>��̃m�[�h�C���f�b�N�X</returns>
		int GetTo() const noexcept { return m_to; }
	};

}