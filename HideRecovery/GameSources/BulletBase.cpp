
/*!
@file StatorBase.cpp
@brief StatorBaseのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "BulletBase.h"

namespace basecross {

	BulletBase::BulletBase(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr)
	{}

}