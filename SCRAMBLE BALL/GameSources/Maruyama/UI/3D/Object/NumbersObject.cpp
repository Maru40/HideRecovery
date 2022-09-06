
/*!
@file NumbersObject.cpp
@brief NumbersObjectクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "NumbersObject.h"
#include "../Component/NumbersCtrl.h"
#include "Maruyama/Utility/Component/BillBoard.h"

namespace basecross {

	void NumbersObject::OnCreate() {
		AddComponent<NumbersCtrl>();

		AddComponent<BillBoard>();
	}

}

//endbasecross