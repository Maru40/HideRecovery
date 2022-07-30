
/*!
@file SkillBase.cpp
@brief SkillBaseのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "SkillBase.h"

namespace basecross {

	SkillBase::SkillBase(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr)
	{}

}

//endbasecross