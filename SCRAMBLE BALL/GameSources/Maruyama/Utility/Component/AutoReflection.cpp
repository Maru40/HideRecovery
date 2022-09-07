/*!
@file AutoReflection.cpp
@brief AutoReflection����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "AutoReflection.h"

#include "VelocityManager.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Utility/UtilityVelocity.h"
#include "Maruyama/Utility/UtilityObstacle.h"

namespace basecross {

	AutoReflection::AutoReflection(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr)
	{}

	void AutoReflection::OnCollisionEnter(const CollisionPair& pair) {
		auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);

		// Velocity������A��Q���Ȃ�
		auto other = pair.m_Dest.lock()->GetGameObject();
		if (velocityManager) { 
			auto velocity = UtilityVelocity::Reflection(velocityManager->GetVelocity(), pair.m_SrcHitNormal);
			velocityManager->SetVelocity(velocity);
		}
	}

}