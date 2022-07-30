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
		float value;      //�_���[�W�l
		Vec3 knockVec;    //�m�b�N�o�b�N����
		float knockPower; //�m�b�N�o�b�N�p���[
		float slowTime;   //�x������
		Vec3 hitPoint;    //���������ꏊ

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		DamageData();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="value">�_���[�W�l</param>
		DamageData(const float& value);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="value">�_���[�W�l</param>
		/// <param name="knockVec">�m�b�N�o�b�N����</param>
		/// <param name="knockPower">�m�b�N�o�b�N�p���[</param>
		/// <param name="slowTime">�x������</param>
		DamageData(const float& value, const Vec3& knockVec, const float& knockPower, const float& slowTime);

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