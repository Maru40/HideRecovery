
/*!
@file Respawner.cpp
@brief Respawner�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Respawner.h"

#include "PlayerSpawnPoint.h"

namespace basecross {

	Respawner::Respawner(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr)
	{}

	void Respawner::OnCreate() {

	}

	void Respawner::Respawn() {
		if (IsRespawn()) {
			transform->SetPosition(GetSpawnPoint()->GetWorldPosition());
		}
	}

	bool Respawner::IsRespawn() {
		return GetSpawnPoint() ? true : false;	//�X�|�[���|�C���g������Ȃ�true
	}

}