﻿#include "AIActiveChanger.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Utility/Component/SeekTarget.h"
#include "Maruyama/Enemy/Component/SelfAstarNodeController.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Enemy/Component/AIVirtualController.h"
#include "Maruyama/Enemy/Component/Stator/AIPlayerStator.h"

namespace basecross
{
	AIActiveChanger::AIActiveChanger(const std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{

	}

	void AIActiveChanger::OnLateStart()
	{
		auto ownerObject = GetGameObject();

		m_seekTarget = ownerObject->GetComponent<SeekTarget>();
		m_selfAStarNodeController = ownerObject->GetComponent<SelfAstarNodeController>();
		m_targetManager = ownerObject->GetComponent<TargetManager>();
		m_aiVertualController = ownerObject->GetComponent<AIVirtualController>();
		m_enemyBase = ownerObject->GetComponent<Enemy::EnemyBase>();
		m_aiPlayerStator = ownerObject->GetComponent<Enemy::AIPlayerStator>();
	}

	void AIActiveChanger::AIActiveChange(bool isActive)
	{
		m_seekTarget.lock()->SetUpdateActive(isActive);
		m_selfAStarNodeController.lock()->SetUpdateActive(isActive);
		m_targetManager.lock()->SetUpdateActive(isActive);
		m_aiVertualController.lock()->SetUpdateActive(isActive);
		m_enemyBase.lock()->SetUpdateActive(isActive);
		m_aiPlayerStator.lock()->SetUpdateActive(isActive);
	}
}