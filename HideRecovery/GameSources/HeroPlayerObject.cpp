
/*!
@file HeroPlayerObject.cpp
@brief HeroPlayerObject�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "PlayerObject.h"
#include "HeroPlayerObject.h"

namespace basecross {

	HeroPlayerObject::HeroPlayerObject(const std::shared_ptr<Stage>& stage):
		PlayerObject(stage)
	{}

	void HeroPlayerObject::OnCreate() {
		PlayerObject::OnCreate();
	}

}