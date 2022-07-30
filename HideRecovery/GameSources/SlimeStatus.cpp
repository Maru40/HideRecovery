
/*!
@file SlimeStatus.cpp
@brief SlimeStatus�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyStatusBase.h"
#include "SlimeStatus.h"

namespace basecross {
	namespace Enemy {

		SlimeStatusManager::SlimeStatusManager(const std::shared_ptr<GameObject>& objPtr)
			:SlimeStatusManager(objPtr, EnemyStatus())
		{}

		SlimeStatusManager::SlimeStatusManager(const std::shared_ptr<GameObject>& objPtr, const EnemyStatus& status)
			: EnemyStatusManagerBase(objPtr, status)
		{}

	}
}