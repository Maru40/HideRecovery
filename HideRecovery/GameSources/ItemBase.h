/*!
@file ItemBase.h
@brief ItemBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �A�C�e���̊��N���X
	//--------------------------------------------------------------------------------------
	class ItemBase : public Component
	{
		bool m_isAcquisition;		//�l���ł����Ԃ��ǂ���

	public:
		ItemBase(const std::shared_ptr<GameObject>& objPtr);

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �l���ł����Ԃ��ǂ���
		/// </summary>
		/// <returns>�l���ł����ԂȂ�true</returns>
		bool IsAcquisition() const;

		/// <summary>
		/// �l���ł����Ԃ��ǂ����̐ݒ�
		/// </summary>
		/// <param name="isAcquisition">�l���ł����Ԃ��ǂ���</param>
		void SetIsAcquisition(const bool isAcquisition);
	};

}