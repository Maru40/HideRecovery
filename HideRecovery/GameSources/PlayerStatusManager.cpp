/*!
@file PressCenterManager.cpp
@brief PressCenterManager�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "I_Damaged.h"

#include "I_Pressed.h"
#include "I_Pusher.h"

#include "PlayerStatusManager.h"

#include "GameManager.h"
#include "GameTimer.h"

#include "Animator_Player.h"

#include "DebugObject.h"
#include "TimeManager.h"
#include "PlayerController.h"

#include "EffectManager.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �v���C���[�X�e�[�^�X
	//--------------------------------------------------------------------------------------

	PlayerStatus::PlayerStatus()
		:PlayerStatus(100.0f, 100.0f, 40.0f, 90.0f, false)
	{}

	PlayerStatus::PlayerStatus(const float& maxHP, const float& energy, const float& energyRecovery, const float& energyUsage, const bool& isUnrivaled):
		hp(maxHP), maxHP(maxHP),
		energy(energy), energyRecovery(energyRecovery), energyUsage(energyUsage),
		isUnrivaled(isUnrivaled)
	{}

	//--------------------------------------------------------------------------------------
	/// �v���C���[�X�e�[�^�X�Ǘ�
	//--------------------------------------------------------------------------------------

	PlayerStatusManager::PlayerStatusManager(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr), m_status(PlayerStatus()), m_timer(new GameTimer(0.0f)), m_damageIntervalTime(3.0f)
	{}

	void PlayerStatusManager::OnUpdate() {
		m_timer->UpdateTimer();
		if (m_timer->IsTimeUp()) {
			m_status.isUnrivaled = false;
		}
		
		RecoveryEnergy();
	}

	void PlayerStatusManager::Damaged(const DamageData& data) {
		if (m_status.hp <= 0) {
			return;
		}

		if (!m_status.isUnrivaled)
		{
			//�_���[�W�p�[�e�B�N��
			if(auto effectManager = EffectManager::GetInstance()){
				effectManager->Play(EffectManager::ID::TackleDamage, data.hitPoint);
			}

			// HP�����炷
			m_status.hp -= data.value;
			// HP��0�ȉ��Ȃ�Q�[���I�[�o�[
			if (m_status.hp <= 0) {
				m_status.hp = 0;
				//���S�A�j���[�V�����ɕύX
				auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
				if (animator) {
					animator->ForceChangeState(PlayerAnimator::StateEnum::Death);
				}

				//�R���g���[���[Off
				auto controller = GetGameObject()->GetComponent<PlayerController>();
				if (controller) {
					controller->SetUpdateActive(false);
				}

				return;
			}

			// �m�b�N�o�b�N
			auto transComp = this->GetGameObject()->GetComponent<Transform>();
			auto pos = transComp->GetPosition();
			pos = pos + data.knockVec * data.knockPower;
			transComp->SetPosition(pos);

			//�_���[�W�C���^�[�o���J�E���g
			m_status.isUnrivaled = true;
			m_timer->ResetTimer(m_damageIntervalTime);

			//�_���[�W�A�j���[�V�����Đ�
			auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
			if (animator) {
				animator->ChangeState(PlayerAnimator::StateEnum::Damage);
			}

			//�_���[�W���̍s���s�\
			auto timer = GetGameObject()->GetComponent<TimeManager>(false);
			if (timer)
			{
				auto controller = GetGameObject()->GetComponent<PlayerController>();
				if (controller)
				{
					controller->SetUpdateActive(false);
					const std::function<void()>& func = [&, controller]() { controller->SetUpdateActive(true); };
					auto data = TimeData(L"Dameged", 1.0f, func);
					timer->AddTimer(data);
				}
			}
		}
	}

	void PlayerStatusManager::ReduceEnergy(float energy)
	{
		m_status.energy -= energy; //�G�l���M�[����

		auto time = this->GetGameObject()->GetComponent<TimeManager>(false);
		if (time) //�k���`�F�b�N
		{
			auto data =  TimeData(L"EnergyRecoverFlag", 0.7f, &m_RecoverEnergy);
			time->ReSetTimer(data);
		}
		m_RecoverEnergy = false;
	}

	void PlayerStatusManager::EnergyRateTime(float rate, float time)
	{
		SetEnergyRecoverRate(rate); //���[�g��ݒ�

		auto timer = GetGameObject()->GetComponent<TimeManager>();
		if (timer)
		{
			// ���Ԍo�ߌヌ�[�g�����ɖ߂�
			auto data = TimeData(L"EnergyRateTime", time, [&]() {SetEnergyRecoverRate(1.0f); });
			timer->AddTimer(data);
		}
	}

	void PlayerStatusManager::RecoveryEnergy()
	{
		if (m_RecoverEnergy)
		{
			// �A�v���̎Q�Ǝ擾
			auto& app = App::GetApp();

			// �P�ʎ��Ԃ̎擾
			float deltaTime = app->GetElapsedTime();

			if (m_status.energy < 100.0f)
			{
				m_status.energy += m_status.energyRecovery * deltaTime * m_EnergyRecoverRate; // �G�l���M�[�Q�[�W��
			}

			if (m_status.energy > 100.0f) // ����l���z�����ꍇ�͕␳���s��
			{
				m_status.energy = 100.0f;
			}
			else if (m_status.energy < 0.0f) // �����l����������ꍇ�����l�ɕ␳���s��
			{
				m_status.energy = 0.0f;
			}
		}
	}

	void PlayerStatusManager::Press(const PressedEnergy& energy) {
		m_status.energy += energy.value;
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void PlayerStatusManager::SetStatus(const Status& status) {
		m_status = status;
	}

	PlayerStatusManager::Status PlayerStatusManager::GetStatus() const {
		return m_status;
	}

	bool PlayerStatusManager::IsDeath() const {
		return m_status.hp <= 0;
	}

	bool PlayerStatusManager::IsUseEnergy() const {
		return m_status.energy > 0; //�G�l���M�[��0���傫���Ȃ�
	}

}

//endbasecross