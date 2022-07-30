/*!
@file I_BrackBoard.h
@brief I_BrackBoard�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �u���b�N�{�[�h�̃C���^�[�t�F�[�X
	//--------------------------------------------------------------------------------------
	template<class Struct>
	class I_BrackBoard
	{
		Struct m_blackBoardParam;  //�u���b�N�{�[�h

	public:
		/// <summary>
		/// �u���b�N�{�[�h�̃p�����[�^�̐ݒ�
		/// </summary>
		/// <param name="param">�u���b�N�{�[�h�̃p�����[�^</param>
		virtual void SetBlackBoardParam(const Struct& param) {
			m_blackBoardParam = param;
		}

		/// <summary>
		/// �u���b�N�{�[�h�̃p�����[�^�̎擾
		/// </summary>
		/// <returns>�u���b�N�{�[�h�̃p�����[�^</returns>
		virtual Struct GetBlackBoardParam() const {
			return m_blackBoardParam;
		}

		/// <summary>
		/// �u���b�N�{�[�h�̃p�����[�^�̎Q�Ƃ��擾
		/// </summary>
		/// <returns>�u���b�N�{�[�h�̃p�����[�^�̎Q��</returns>
		virtual Struct& GetRefBlackBoardParam() {
			return m_blackBoardParam;
		}
	};

}

//basecross