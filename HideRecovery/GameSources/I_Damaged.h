/*!
@file I_Damaged.h
@brief I_Damaged�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// �_���[�W�f�[�^
	//--------------------------------------------------------------------------------------
	struct DamageData
	{
		int value;      //�_���[�W�l

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		DamageData();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="value">�_���[�W�l</param>
		DamageData(const int& value);

		DamageData operator+(const DamageData& data);

		DamageData operator+=(const DamageData& data);
	};

	class I_Damaged
	{
	public:
		/// <summary>
		/// �_���[�W���󂯂鏈��
		/// </summary>
		/// <param name="data">�_���[�W�f�[�^</param>
		virtual void Damaged(const DamageData& data) = 0;
	};
}

//endbasecorss