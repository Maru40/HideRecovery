
/*!
@file NumberObject.cpp
@brief NumberObjectクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "DebugNumber.h"
#include "NumberObject.h"

namespace basecross {

	void NumberObject::OnCreate() {
		AddComponent<maru::Number>(0);
	}

}

//endbasecross