/*!
@file TargetManager.h
@brief TargetManager
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�^�[�Q�b�g�Ǘ�
	//--------------------------------------------------------------------------------------
	class TargetManager : public Component
	{
	public:
		/// <summary>
		/// ���������^�[�Q�b�g�̃f�[�^
		/// </summary>
		struct LostData;

		/// <summary>
		/// �p�����[�^
		/// </summary>
		struct Parametor;

	private:
		std::unique_ptr<Parametor> m_paramPtr;	//�p�����[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		TargetManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �^�[�Q�b�g�����݂��邩�ǂ���
		/// </summary>
		/// <returns>�^�[�Q�b�g�����݂���Ȃ�true</returns>
		bool HasTarget() const;

		/// <summary>
		/// �^�[�Q�b�g�̈ʒu���擾����
		/// </summary>
		/// <returns>�^�[�Q�b�g�̈ʒu</returns>
		Vec3 GetTargetPosition() const;

		/// <summary>
		/// �^�[�Q�b�g�����������ʒu���擾����
		/// </summary>
		/// <returns>�^�[�Q�b�g�����������ʒu</returns>
		Vec3 GetLostPosition() const;
	
		/// <summary>
		/// �^�[�Q�b�g�̐ݒ�
		/// </summary>
		/// <param name="target">�^�[�Q�b�g</param>
		void SetTarget(const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// �^�[�Q�b�g�̎擾
		/// </summary>
		/// <returns>�^�[�Q�b�g</returns>
		std::shared_ptr<GameObject> GetTarget() const;

		/// <summary>
		/// �^�[�Q�b�g�����̃x�N�g����Ԃ��B
		/// </summary>
		/// <returns>�^�[�Q�b�g�����̃x�N�g��</returns>
		Vec3 CalcuToTargetVec();
	};

}

//endbasecrosss