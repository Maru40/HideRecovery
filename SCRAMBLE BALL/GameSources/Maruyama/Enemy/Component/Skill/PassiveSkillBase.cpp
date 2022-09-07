
/*!
@file PassiveSkillBase.cpp
@brief PassiveSkillBaseのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "SkillBase.h"
#include "PassiveSkillBase.h"

namespace basecross {

	PassiveSkillBase::PassiveSkillBase(const std::shared_ptr<GameObject>& objPtr)
		:SkillBase(objPtr)
	{}

}

//endbasecross