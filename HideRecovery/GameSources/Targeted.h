/*!
@file Targeted.h
@brief Targeted�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �^�[�Q�b�g�ɂ����Ώۂ̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct Targeted_Parametor
	{
		Vec3 offset = Vec3(0.0f);
	};

	//--------------------------------------------------------------------------------------
	/// �^�[�Q�b�g�ɂ����Ώ�
	//--------------------------------------------------------------------------------------
	class Targeted : public Component
	{
	public:
		using Parametor = Targeted_Parametor;

	private:
		Parametor m_param; //�p�����[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		Targeted(const std::shared_ptr<GameObject>& objPtr);
		
	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �I�t�Z�b�g���l�������^�[�Q�b�g�ʒu���擾
		/// </summary>
		/// <returns>�I�t�Z�b�g���l�������^�[�Q�b�g�̈ʒu</returns>
		Vec3 GetPosition() const;

		/// <summary>
		/// �^�[�Q�b�g�ʒu�̃I�t�Z�b�g��ݒ�
		/// </summary>
		/// <param name="offset">�^�[�Q�b�g�ʒu�̃I�t�Z�b�g</param>
		void SetOffset(const Vec3& offset);

		/// <summary>
		/// �^�[�Q�b�g�ʒu�̃I�t�Z�b�g���擾
		/// </summary>
		/// <returns>�^�[�Q�b�g�ʒu�̃I�t�Z�b�g</returns>
		Vec3 GetOffset() const;
	};

}