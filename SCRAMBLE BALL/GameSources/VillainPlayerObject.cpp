
/*!
@file VillainPlayerObject.cpp
@brief VillainPlayerObject�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "PlayerObject.h"
#include "VillainPlayerObject.h"

#include "Maruyama/Player/Component/OwnHideItemManager.h"

namespace basecross {

	VillainPlayerObject::VillainPlayerObject(const std::shared_ptr<Stage>& stage) :
		PlayerObject(stage)
	{}

	void VillainPlayerObject::OnCreate() {
		PlayerObject::OnCreate();

		AddComponent<OwnHideItemManager>();
	}

}