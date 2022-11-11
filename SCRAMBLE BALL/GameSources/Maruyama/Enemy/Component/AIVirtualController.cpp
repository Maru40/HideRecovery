
/*!
@file AIVirtualController.cpp
@brief AIVirtualControllerのクラス実体
担当：丸山裕喜
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