/*!
@file PlayerSpawnPoint.cpp
@brief PlayerSpawnPoint�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "PlayerSpawnPoint.h"

namespace basecross {
	PlayerSpawnPoint_Parametor::PlayerSpawnPoint_Parametor(const team::TeamType& team, const int id) :
		team(team),
		id(id)
	{}

	PlayerSpawnPoint::PlayerSpawnPoint(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor) :
		Component(objPtr),
		m_param(parametor)
	{}
}