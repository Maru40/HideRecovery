/*!
@file WeponBase.h
@brief WeponBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class BulletBase;
	class GameTimer;

	enum class Wepon_ShotType {
		Hold, //�z�[���h�V���b�g
	};

	//--------------------------------------------------------------------------------------
	/// �������N���X�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct WeponBase_Parametor {
		using ShotType = Wepon_ShotType;    //�E�F�|���^�C�v

		ShotType shotType = ShotType::Hold; //�V���b�g�^�C�v
		Vec3 positionOffset = Vec3(0.0f);   //�ʒu�I�t�Z�b�g
		float energyConsumed;				//�G�l���M�[�����
		float overheatCoolTime;				//�I�[�o�[�q�[�g�N�[���^�C��
		bool isOverheat;					//�I�[�o�[�q�[�g��Ԃ��ǂ���
		
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		WeponBase_Parametor();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="type">�V���b�g�^�C�v</param>
		/// <param name="positionOffset">�ʒu�I�t�Z�b�g</param>
		WeponBase_Parametor(const ShotType& type, const Vec3& positionOffset);
	};

	//--------------------------------------------------------------------------------------
	/// �������N���X
	//--------------------------------------------------------------------------------------
	class WeponBase : public Component
	{
	public:
		using Parametor = WeponBase_Parametor;
		using ShotType = Wepon_ShotType;

	private:
		std::unique_ptr<GameTimer> m_timer;		//���ԊǗ��N���X

	protected:
		ex_weak_ptr<GameObject> m_holdBullet;	//�Œ�o���b�g
		Parametor m_baseParam;					//���p�����[�^	

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		WeponBase(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="parametor">�p�����[�^</param>
		WeponBase(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		virtual void OnUpdate() override;

		/// <summary>
		/// ������
		/// </summary>
		virtual void Shot();

		/// <summary>
		/// �����I���������
		/// </summary>
		virtual void ShotEnd();

	protected:

		/// <summary>
		/// �z�[���h���Ɍ�����
		/// </summary>
		virtual void HoldShotUpdate() {};

		/// <summary>
		/// �z�[���h����
		/// </summary>
		virtual void HoldShotEnd() {};

		/// <summary>
		/// �G�l���M�[����
		/// </summary>
		virtual void ReduceEnergy();

		/// <summary>
		/// �o���b�g�̐�������ʒu���擾
		/// </summary>
		/// <returns>��������ꏊ</returns>
		Vec3 CalculateCreateBulletPosition();

		/// <summary>
		/// �I�[�o�[�q�[�g
		/// </summary>
		void Overhear();

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �V���b�g�^�C�v�̎擾
		/// </summary>
		/// <returns>�V���b�g�^�C�v</returns>
		ShotType GetShotType() const noexcept { return m_baseParam.shotType; }

		/// <summary>
		/// ����G�l���M�[�̐ݒ�
		/// </summary>
		/// <param name="energyConsumed">����G�l���M�[</param>
		void SetEnergyConsumed(const float energyConsumed) noexcept { m_baseParam.energyConsumed = energyConsumed; }

		/// <summary>
		/// ����G�l���M�[�̎擾
		/// </summary>
		/// <returns>����G�l���M�[</returns>
		float GetEnergyConsumed() const noexcept { return m_baseParam.energyConsumed; }

		/// <summary>
		/// �I�[�o�[�q�[�g���Ă���ǂ�����ݒ�
		/// </summary>
		/// <param name="isOverheart">�I�[�o�[�q�[�g���Ă��邩�ǂ���</param>
		void SetIsOverheat(const bool isOverheart) { m_baseParam.isOverheat = isOverheart; }

		/// <summary>
		/// �I�[�o�[�q�[�g���Ă��邩�ǂ�����Ԃ�
		/// </summary>
		/// <returns>�I�[�o�[�q�[�g���Ă���Ȃ�true</returns>
		bool IsOverheat() const noexcept { return m_baseParam.isOverheat; }

		/// <summary>
		/// �G�l���M�[���g�p�\���ǂ���
		/// </summary>
		/// <returns>�G�l���M�[�g�p�\�Ȃ�true</returns>
		bool IsUseEnergy() const;

		/// <summary>
		/// ���Ă��Ԃ��ǂ���
		/// </summary>
		/// <returns>���Ă��ԂȂ�true</returns>
		bool IsShot() const;

	};

}