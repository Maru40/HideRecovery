/*!
@file BulletBase.h
@brief BulletBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Interface/I_Damaged.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�e�ۂ̊��N���X�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct BulletBase_Parametor {
		Vec3 moveDirect;		//�ړ�����
		float moveSpeed;		//�ړ��X�s�[�h
		float maxRange;			//�ړ��ő勗��
		DamageData damageData;	//�_���[�W�f�[�^

		BulletBase_Parametor();
	};

	//--------------------------------------------------------------------------------------
	///	�e�ۂ̊��N���X
	//--------------------------------------------------------------------------------------
	class BulletBase : public Component
	{
	public:
		using Parametor = BulletBase_Parametor;

	private:
		Parametor m_param;					//�p�����[�^

		std::weak_ptr<GameObject> m_owner;	//���̒e�̏��L��

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		BulletBase(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

		/// <summary>
		/// ������
		/// </summary>
		/// <param name="owner">��������</param>
		/// <param name="direct">������</param>
		virtual void Shot(const std::shared_ptr<GameObject>& owner, const Vec3& direct) {}

		/// <summary>
		/// ���������I�������^�C�~���O�ŌĂԏ���
		/// </summary>
		virtual void ShotEnd() {}

	private:
		/// <summary>
		/// �ړ��X�V
		/// </summary>
		void MoveUpdate();

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		void SetParametor(const Parametor& parametor) noexcept { m_param = parametor; }

		Parametor GetParametor() const noexcept { return m_param; }

		/// <summary>
		/// �ړ������̐ݒ�
		/// </summary>
		/// <param name="direct">�ړ�����</param>
		void SetMoveDirect(const Vec3& direct) noexcept { m_param.moveDirect = direct; }

		/// <summary>
		/// �ړ������̎擾
		/// </summary>
		/// <returns>�ړ�����</returns>
		Vec3 GetMoveDirect() const noexcept { return m_param.moveDirect; }

		void SetMoveSpeed(const float speed) noexcept { m_param.moveSpeed = speed; }

		/// <summary>
		/// �ړ��ő勗���̐ݒ�
		/// </summary>
		/// <param name="range">�ړ��ő勗��</param>
		void SetMaxRange(const float range) noexcept { m_param.maxRange = range; }

		/// <summary>
		/// �ړ��ő勗���̎擾
		/// </summary>
		/// <returns>�ړ��ő勗��</returns>
		float GetMaxRange() const noexcept { return m_param.maxRange; }

		/// <summary>
		/// �_���[�W�f�[�^�̐ݒ�
		/// </summary>
		/// <param name="data">�_���[�W�f�[�^</param>
		void SetDamageData(const DamageData& data) noexcept { m_param.damageData = data; }

		/// <summary>
		/// �_���[�W�f�[�^�̎擾
		/// </summary>
		/// <returns>�_���[�W�f�[�^</returns>
		DamageData GetDamageData() const noexcept { return m_param.damageData; }

		/// <summary>
		/// ���L�҂̐ݒ�
		/// </summary>
		/// <param name="owner">���L��</param>
		void SetOwner(const std::shared_ptr<GameObject>& owner) noexcept { m_owner = owner; }

		/// <summary>
		/// ���L�҂̎擾
		/// </summary>
		/// <returns>���L��</returns>
		std::shared_ptr<GameObject> GetOwner() const { return m_owner.lock(); }
	};

}

//endbasecross