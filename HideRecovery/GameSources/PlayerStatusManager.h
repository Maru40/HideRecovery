/*!
@file Pusher.h
@brief Pusher�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	struct DamageData;
	class I_Damaged;

	struct PressedEnergy;
	class I_Pusher;

	class GameTimer;

	//--------------------------------------------------------------------------------------
	/// �v���C���[�̃X�e�[�^�X
	//--------------------------------------------------------------------------------------
	struct PlayerStatus
	{
		float hp;             // �v���C���[��HP // 100
		float maxHP;          // MaxHP

		float energy;         // �^�b�N���p�̃G�l���M�[ // 100
		float energyRecovery; // �G�l���M�[�񕜗� // 10/�b
		float energyUsage;    // �G�l���M�[����� // 50/�b

		bool isUnrivaled;     //���G��Ԃ��ǂ���

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		PlayerStatus();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="maxHp">�ő�HP</param>
		/// <param name="energy">�G�l���M�[</param>
		/// <param name="energyRecovery">�G�l���M�[�񕜗�</param>
		/// <param name="energyUsage">�G�l���M�[�����</param>
		/// <param name="isUnrivaled">���G��Ԃ��ǂ���</param>
		PlayerStatus(const float& maxHp, const float& energy, const float& energyRecovery, const float& energyUsage, const bool& isUnrivaled);
	};

	//--------------------------------------------------------------------------------------
	/// �v���C���[�̃X�e�[�^�X�Ǘ�
	//--------------------------------------------------------------------------------------
	class PlayerStatusManager : public Component, public I_Damaged, public I_Pusher
	{
	public:
		using Status = PlayerStatus;

	protected:
		Status m_status;                    //�X�e�[�^�X
		std::unique_ptr<GameTimer> m_timer; //�^�C���Ǘ��N���X
		float m_damageIntervalTime = 5.0f;  //�_���[�W���󂯂���̖��G����
		bool m_RecoverEnergy = true;        //Energy�����񕜂��ł��邩�ǂ���
		float m_EnergyRecoverRate = 1.0f;   //�����񕜂̃��[�g

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		PlayerStatusManager(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

		//--------------------------------------------------------------------------------------
		/// �C���^�[�t�F�[�X�̎���
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �_���[�W���󂯂�
		/// </summary>
		/// <param name="data">�_���[�W�f�[�^</param>
		void Damaged(const DamageData& data) override;

		/// <summary>
		/// �G�l���M�[�̏���
		/// </summary>
		/// <param name="energy">����G�l���M�[</param>
		void ReduceEnergy(float energy);

		/// <summary>
		/// �񕜗ʌ���
		/// </summary>
		/// <param name="rate">���۔{��</param>
		/// <param name="time">����</param>
		void EnergyRateTime(float rate, float time);

		/// <summary>
		/// �G�l���M�[�̉�
		/// </summary>
		void RecoveryEnergy();

		/// <summary>
		/// �ׂ�����
		/// </summary>
		/// <param name="energy">�ׂ��G�l���M�[</param>
		void Press(const PressedEnergy& energy) override;

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �X�e�[�^�X�̐ݒ�
		/// </summary>
		/// <param name="status"></param>
		void SetStatus(const Status& status);

		/// <summary>
		/// �X�e�[�^�X�̎擾
		/// </summary>
		/// <returns>�X�e�[�^�X</returns>
		Status GetStatus() const;

		/// <summary>
		/// �G�l���M�[�񕜗ʂ�ݒ�
		/// </summary>
		/// <param name="rate">�G�l���M�[�񕜗�</param>
		void SetEnergyRecoverRate(float rate) {
			m_EnergyRecoverRate = rate;
		}

		/// <summary>
		/// ����ł����Ԃ��ǂ�����Ԃ��B
		/// </summary>
		/// <returns>����ł����Ԃ��ǂ���</returns>
		bool IsDeath() const;

		/// <summary>
		/// �G�l���M�[���g�����Ԃ��ǂ���
		/// </summary>
		/// <returns>�g����Ȃ�true</returns>
		bool IsUseEnergy() const;
	};

}