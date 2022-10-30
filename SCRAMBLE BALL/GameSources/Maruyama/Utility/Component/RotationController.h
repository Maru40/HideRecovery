/*!
@file RotationController.h
@brief RotationController
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	��]�R���g���[��
	//--------------------------------------------------------------------------------------
	class RotationController : public Component
	{
	public:
		/// <summary>
		/// �p�����[�^
		/// </summary>
		struct Parametor
		{
			float speed;			//��]���x
 			Vec3 direction;			//������������
			bool isUseY = false;	//Y�����l�����邩�ǂ���

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="speed">��]���x</param>
			/// <param name="isUseY">Y�����l�����邩�ǂ���</param>
			Parametor(const float& speed, const bool isUseY = false);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="speed">��]���x</param>
			/// <param name="direction">������������</param>
			/// <param name="isUseY">Y�����l�����邩�ǂ���</param>
			Parametor(const float& speed, const Vec3& direction, const bool isUseY = false);
		};

	private:
		Parametor m_param;	//�p�����[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		RotationController(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ��]�����ǂ�����Ԃ�
		/// </summary>
		/// <returns>��]���Ȃ�true</returns>
		bool IsRotation();

		/// <summary>
		/// ��������������ݒ�
		/// </summary>
		/// <param name="direction">������������</param>
		void SetDirection(const Vec3& direction) noexcept { m_param.direction = direction; }

		/// <summary>
		/// �������������̎擾
		/// </summary>
		/// <returns>������������</returns>
		Vec3 GetDirection() const noexcept { return m_param.direction; }

		/// <summary>
		/// ��]���x�̐ݒ�
		/// </summary>
		/// <param name="speed">��]���x</param>
		void SetSpeed(const float& speed) noexcept { m_param.speed = speed; }

		/// <summary>
		/// ��]���x�̎擾
		/// </summary>
		/// <returns>��]���x</returns>
		float GetSpeed() const noexcept { return m_param.speed; }

		/// <summary>
		/// Y�����l�����邩�ǂ����̐ݒ�
		/// </summary>
		/// <param name="isUseY">Y�����l������Ȃ�true</param>
		void SetIsUseY(const bool isUseY) noexcept { m_param.isUseY = isUseY; }

		/// <summary>
		/// Y�����l�����邩�ǂ�����Ԃ�
		/// </summary>
		/// <returns>Y�����l������Ȃ�true</returns>
		bool IsUseY() const noexcept { return m_param.isUseY; }

		/// <summary>
		/// �p�����[�^�̐ݒ�
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

//endbasecross