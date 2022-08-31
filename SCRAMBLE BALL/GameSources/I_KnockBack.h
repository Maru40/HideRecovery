/*!
@file I_KnockBack.h
@brief I_KnockBack�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �m�b�N�o�b�N�f�[�^
	//--------------------------------------------------------------------------------------
	struct KnockBackData {
		float power;   //�m�b�N�o�b�N��
		Vec3 hitPoint; //�Փˈʒu
		Vec3 direct;   //�m�b�N�o�b�N����

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		KnockBackData();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="power">�m�b�N�o�b�N��</param>
		/// <param name="hitPoint">�Փˈʒu</param>
		/// <param name="direct">�m�b�N�o�b�N����</param>
		KnockBackData(const float& power, const Vec3& hitPoint, const Vec3& direct);
	};

	//--------------------------------------------------------------------------------------
	/// �m�b�N�o�b�N�C���^�[�t�F�[�X
	//--------------------------------------------------------------------------------------
	class I_KnockBack
	{
	public:
		/// <summary>
		/// �m�b�N�o�b�N����鏈��
		/// </summary>
		/// <param name="data">�m�b�N�o�b�N�f�[�^</param>
		virtual void KnockBack(const KnockBackData& data) = 0;

	};

}

//endbasecross