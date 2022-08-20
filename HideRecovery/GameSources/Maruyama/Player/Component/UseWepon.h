
/*!
@file UseWepon.h
@brief UseWepon�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class WeponBase;
	class RotationController;

	namespace maru {
		class ReactiveBool;
	}

	//--------------------------------------------------------------------------------------
	/// �E�F�|�����g�p����N���X�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct UseWepon_Parametor {
		std::unique_ptr<maru::ReactiveBool> isAim;	//Aim�����ǂ���

		UseWepon_Parametor();

		UseWepon_Parametor(const bool isAim);

		/// <summary>
		/// �R�s�[�R���X�g���N�^
		/// </summary>
		UseWepon_Parametor(const UseWepon_Parametor& parametor);

		virtual ~UseWepon_Parametor() = default;
	};

	//--------------------------------------------------------------------------------------
	/// �E�F�|�����g�p����N���X
	//--------------------------------------------------------------------------------------
	class UseWepon : public Component
	{
	public:
		using Parametor = UseWepon_Parametor;

	private:
		Parametor m_param;					//�p�����[�^

		std::weak_ptr<WeponBase> m_wepon;	//����

		std::weak_ptr<RotationController> m_rotationController;	//��]�R���g���[���[

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		UseWepon(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="wepon">����</param>
		UseWepon(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<WeponBase>& wepon);

		virtual ~UseWepon() = default;

		void OnLateStart() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// �G�C�����̍X�V����
		/// </summary>
		void AimUpdate();

		/// <summary>
		/// ��]�X�V
		/// </summary>
		void RotationUpdate();

		/// <summary>
		/// �G�C����Ԃ�؂�ւ����Ƃ��ɌĂяo�����������̐ݒ�
		/// </summary>
		void SettingReactiveIsAim();

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �G�C�����̐ݒ�
		/// </summary>
		/// <param name="isAim">�G�C�������ǂ���</param>
		void SetIsAim(const bool isAim);

		/// <summary>
		/// �G�C�������ǂ���
		/// </summary>
		/// <returns>�G�C�����Ȃ�true</returns>
		bool IsAim() const;

		/// <summary>
		/// ����̐ݒ�
		/// </summary>
		/// <param name="wepon">����</param>
		void SetWepon(const std::shared_ptr<WeponBase>& wepon) noexcept;

		/// <summary>
		/// ����̎擾
		/// </summary>
		/// <returns>����</returns>
		std::shared_ptr<WeponBase> GetWepon() const noexcept;
		
	};

}