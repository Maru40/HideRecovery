
/*!
@file AIVirtualController.cpp
@brief AIVirtualController�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "AIVirtualController.h"

namespace basecross {

	AIVirtualController::AIVirtualController(const std::shared_ptr<GameObject>& objPtr):
		Component(objPtr),
		m_inputDirection(Vec2(0.0f))
	{}

	void AIVirtualController::OnUpdate() {

	}

	void AIVirtualController::SetInputDirection(const Vec2& direction) {
		m_inputDirection = direction;
	}

}