
/*!
@file BuffController.cpp
@brief BuffController�N���X����
�S���F�ێR�T��
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