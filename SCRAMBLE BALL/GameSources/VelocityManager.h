/*!
@file VelocityManager.h
@brief VelocityManager
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	���x�Ǘ�
	//--------------------------------------------------------------------------------------
	class VelocityManager : public Component
	{
	public :
		/// <summary>
		/// �p�����[�^
		/// </summary>
		struct Parametor
		{
			Vec3 force;					//�������
			Vec3 velocity;				//���x       
			Vec3 beforeVelocity;		//�ʒu�t���[���O�̑��x

			float deselerationPower;	//�������x
			float maxSpeed;				//�ő�X�s�[�h
			bool isDeseleration;		//���������ǂ���

			Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="force">�������</param>
			/// <param name="velocity">���x</param>
			/// <param name="deselerationPower">�������x</param>
			/// <param name="isDeseleration">���������ǂ���</param>
			Parametor(const Vec3& force, const Vec3& velocity, const float& deselerationPower, const bool isDeseleration = false);
		};

	private:
		Parametor m_param;	//�p�����[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		VelocityManager(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

	private:
		/// <summary>
		/// �ړ�����
		/// </summary>
		void Move();

		/// <summary>
		/// ��������
		/// </summary>
		void Deseleration();

		/// <summary>
		/// �d�͏����X�V
		/// </summary>
		void GravityUpdate();

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ���x�̐ݒ�
		/// </summary>
		/// <param name="velocity">���x</param>
		void SetVelocity(const Vec3& velocity) noexcept;

		/// <summary>
		/// ���x�̎擾
		/// </summary>
		/// <returns>���x</returns>
		Vec3 GetVelocity() const noexcept;

		/// <summary>
		/// ���[���h���x�̎擾
		/// </summary>
		/// <returns>���[���h���x</returns>
		Vec3 GetWorldVelocity() const;

		/// <summary>
		/// 1�t���[���O�̑��x
		/// </summary>
		/// <returns>1�t���[���O�̑��x</returns>
		Vec3 GetBeforeVelocity() const noexcept;

		/// <summary>
		/// �͂�������
		/// </summary>
		/// <param name="force">�ǉ����鑬�x</param>
		void AddForce(const Vec3& force) noexcept;

		/// <summary>
		/// ������͂̎擾
		/// </summary>
		/// <returns>�������</returns>
		Vec3 GetForce() const noexcept;

		/// <summary>
		/// ���x�̃��Z�b�g
		/// </summary>
		void ResetVelocity() noexcept;

		/// <summary>
		/// ������͂̃��Z�b�g
		/// </summary>
        void ResetForce() noexcept;

		/// <summary>
		/// ���x�Ɖ�����͂̃��Z�b�g
		/// </summary>
        void ResetAll() noexcept;

        /// <summary>
        /// �����J�n
        /// </summary>
        /// <param name="power">���������</param>
        void StartDeseleration(const float& power = 1.0f) noexcept;

		/// <summary>
		/// �������Ă��邩�ǂ����̐ݒ�
		/// </summary>
		/// <param name="isDeseleration">�������Ă��邩�ǂ���</param>
        void SetIsDeseleration(const bool isDeseleration) noexcept;

		/// <summary>
		/// �������Ă��邩�ǂ���
		/// </summary>
		/// <returns>�������Ă����ԂȂ�true</returns>
		bool IsDeseleration() const noexcept;

        /// <summary>
        /// �����̋���
        /// </summary>
		float GetDeselerationPower() const noexcept;

		/// <summary>
		/// �����̋���
		/// </summary>
		void SetDeseletationPower(const float& power) noexcept;

		/// <summary>
		/// �ő呬�x
		/// </summary>
		/// <param name="speed">�ő呬�x</param>
		void SetMaxSpeed(const float& speed) noexcept;

		/// <summary>
		/// �ő呬�x�̎擾
		/// </summary>
		/// <returns>�ő呬�x</returns>
		float GetMaxSpeed() const noexcept;
	};

}

//endbasecross