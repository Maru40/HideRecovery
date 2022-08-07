
/*!
@file VillainPlayerObject.cpp
@brief VillainPlayerObjectクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "PlayerObject.h"
#include "VillainPlayerObject.h"

#include "OwnHideItemManager.h"

namespace basecross {

	VillainPlayerObject::VillainPlayerObject(const std::shared_ptr<Stage>& stage) :
		PlayerObject(stage)
	{}

	void VillainPlayerObject::OnCreate() {
		PlayerObject::OnCreate();

		AddComponent<OwnHideItemManager>();
	}

}