
/*!
@file Teleport.cpp
@brief Teleportクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Teleport.h"

namespace basecross {

	Teleport::Teleport(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr)
	{}

	void Teleport::OpenMap() {

	}
}