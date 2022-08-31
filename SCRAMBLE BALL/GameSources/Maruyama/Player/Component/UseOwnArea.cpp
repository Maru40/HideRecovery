
/*!
@file UseOwnArea.cpp
@brief UseOwnAreaクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "UseOwnArea.h"

namespace basecross {

	UseOwnArea::UseOwnArea(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr)
	{}

	void UseOwnArea::OnUpdate() {

	}

}