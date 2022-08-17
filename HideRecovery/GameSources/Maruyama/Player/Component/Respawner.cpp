
/*!
@file Respawner.cpp
@brief Respawnerクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Respawner.h"

namespace basecross {

	Respawner::Respawner(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr)
	{}

	void Respawner::OnCreate() {

	}

}