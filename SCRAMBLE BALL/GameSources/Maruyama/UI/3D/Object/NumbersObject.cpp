
/*!
@file NumbersObject.cpp
@brief NumbersObject�N���X����
�S���F�ێR�T��
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