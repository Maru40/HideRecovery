
/*!
@file ActiveSkillBase.cpp
@brief ActiveSkillBaseのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "SkillBase.h"
#include "ActiveSkillBase.h"

namespace basecross {

	ActiveSkillBase::ActiveSkillBase(const std::shared_ptr<GameObject>& objPtr)
		:SkillBase(objPtr)
	{}

}

//endbasecross