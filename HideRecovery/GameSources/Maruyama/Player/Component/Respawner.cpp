
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
		SetSpawnPoint(maru::Utility::FindComponent<PlayerSpawnPoint>());

		transform->SetPosition(GetSpawnPoint()->GetWorldPosition());
		if (auto status = GetGameObject()->GetComponent<PlayerStatus>(false)) {
			status->Respawn();
		}
	}

	void Respawner::StartRespawn() {
		m_timer->ResetTimer(m_param.time);
	}

	bool Respawner::IsRespawn() {
		return GetSpawnPoint() ? true : false;	//�X�|�[���|�C���g������Ȃ�true
	}

}