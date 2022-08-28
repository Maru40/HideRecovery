/*!
@file Respawner.cpp
@brief Respawner�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Respawner.h"

#include "PlayerSpawnPoint.h"

#include "TimeHelper.h"
#include "GameTimer.h"

#include "Watanabe/Component/PlayerStatus.h"
#include "Watanabe/Component/PlayerAnimator.h"
#include "Watanabe/Effekseer/EfkEffect.h"
#include "Maruyama/Player/Component/UseWepon.h"

#include "MaruUtility.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// ���X�|�[����������N���X�̃p�����[�^
	//--------------------------------------------------------------------------------------

	Respawner_Parametor::Respawner_Parametor() :
		time(2.0f)
	{}

	//--------------------------------------------------------------------------------------
	/// ���X�|�[����������N���X�{��
	//--------------------------------------------------------------------------------------

	Respawner::Respawner(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_param(Parametor()),
		m_timer(new GameTimer(0))
	{}

	void Respawner::OnCreate() {
	}

	void Respawner::OnUpdate() {
		if (m_timer->IsTimeUp()) {
			return;
		}

		m_timer->UpdateTimer();
		if (m_timer->IsTimeUp()) {
			Respawn();
		}
	}

	void Respawner::Respawn() {
		transform->SetPosition(GetSpawnPoint()->GetWorldPosition());

		if (auto status = GetGameObject()->GetComponent<PlayerStatus>(false)) {
			status->Respawn();
		}

		if (auto useWeapon = GetGameObject()->GetComponent<UseWepon>(false)) {
			useWeapon->SetIsAim(false);
		}

		if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {
			animator->ChangePlayerAnimation(PlayerAnimationState::State::Wait);
		}

		//�d�͂�����
		if (auto gravity = GetGameObject()->GetComponent<Gravity>(false)) {
			gravity->SetUpdateActive(true);
		}

		//�����蔻�������
		if (auto collision = GetGameObject()->GetComponent<CollisionObb>(false)) {
			collision->SetUpdateActive(true);
		}

		if (auto efkComp = GetGameObject()->GetComponent<EfkComponent>(false)) {
			efkComp->Play(L"Respawn");
		}
	}

	void Respawner::StartRespawn() {
		m_timer->ResetTimer(m_param.time);
	}

	bool Respawner::IsRespawn() {
		return GetSpawnPoint() ? true : false;	//�X�|�[���|�C���g������Ȃ�true
	}
}