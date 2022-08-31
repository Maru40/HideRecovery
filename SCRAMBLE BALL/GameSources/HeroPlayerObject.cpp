
/*!
@file HeroPlayerObject.cpp
@brief HeroPlayerObjectクラス実体
担当：丸山裕喜
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