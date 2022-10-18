
/*!
@file NumberObject.cpp
@brief NumberObject�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/DebugClass/Component/DebugNumber.h"
#include "NumberObject.h"

namespace basecross {

	void NumberObject::OnCreate() {
		AddComponent<maru::Number>(0);
	}

}

//endbasecross