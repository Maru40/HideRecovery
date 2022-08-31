/*!
@file LookAtCameraManager.h
@brief LookAtCameraManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�J�����̎��_�Ǘ��N���X
	//--------------------------------------------------------------------------------------
	class LookAtCameraManager : public Component
	{
	public:
		struct Parametor 
		{
			float speed;       //�ړ��X�s�[�h
			Vec3 centerOffset; //���S�ʒu�̃I�t�Z�b�g

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="centerOffset">���S�ʒu�̃I�t�Z�b�g</param>
			Parametor(const Vec3& centerOffset);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="speed">�ړ��X�s�[�h</param>
			/// <param name="centerOffset">���S�ʒu�̃I�t�Z�b�g</param>
			Parametor(const float& speed, const Vec3& centerOffset);
		};

	private:
		Parametor m_param;  //�p�����[�^

		ex_weak_ptr<GameObject> m_target = nullptr; //�^�[�Q�b�g�I�u�W�F�N�g�̃|�C���^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		LookAtCameraManager(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="target">�^�[�Q�b�g�̃Q�[���I�u�W�F�N�g</param>
		LookAtCameraManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="target">�^�[�Q�b�g�̃Q�[���I�u�W�F�N�g</param>
		/// <param name="param">�p�����[�^</param>
		LookAtCameraManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GameObject>& target, const Parametor& param);

		void OnUpdate2() override;

	private:
		/// <summary>
		/// ���_�X�V����
		/// </summary>
		void LookAtUpdate();

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �^�[�Q�b�g�̃Z�b�g
		/// </summary>
		/// <param name="target">�^�[�Q�b�g�̃Q�[���I�u�W�F�N�g</param>
		void SetTarget(const std::shared_ptr<GameObject>& target) noexcept { m_target = target; }

		/// <summary>
		/// �^�[�Q�b�g�̎擾
		/// </summary>
		/// <returns>�^�[�Q�b�g�̃Q�[���I�u�W�F�N�g</returns>
		std::shared_ptr<GameObject> GetTarget() const noexcept { return m_target.GetShard(); }

		/// <summary>
		/// �p�����[�^�̃Z�b�g
		/// </summary>
		/// <param name="parametor">�p�����[�^</param>
		void SetParametor(const Parametor& parametor) noexcept { m_param = parametor; }

		/// <summary>
		/// �p�����[�^�̎擾
		/// </summary>
		/// <returns>�p�����[�^</returns>
		Parametor GetParametor() const noexcept { return m_param; }
	};

}

//endbasecorss