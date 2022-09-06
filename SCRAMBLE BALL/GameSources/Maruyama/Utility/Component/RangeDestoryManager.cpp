
/*!
@file RangeDestroyManager.cpp
@brief RangeDestroyManager�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "RangeDestroyManager.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

namespace basecross {

	RangeDestoryManager::RangeDestoryManager(const std::shared_ptr<GameObject>& objPtr, const float& maxRange) :
		Component(objPtr), m_maxRange(maxRange)
	{}

	void RangeDestoryManager::OnCreate() {
		m_initializePosition = transform->GetWorldPosition();
	}

	void RangeDestoryManager::OnUpdate() {
		auto toCurrentPosition = transform->GetWorldPosition() - m_initializePosition;

		if (m_maxRange < toCurrentPosition.length()) {
			GetStage()->RemoveGameObject<GameObject>(GetGameObject());
		}
	}

}