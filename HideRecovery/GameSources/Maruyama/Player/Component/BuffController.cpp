
/*!
@file BuffController.cpp
@brief BuffControllerクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "BuffController.h"

namespace basecross {

	BuffController::BuffController(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_param(Parametor())
	{}

	void BuffController::OnLateStart() {

	}



}