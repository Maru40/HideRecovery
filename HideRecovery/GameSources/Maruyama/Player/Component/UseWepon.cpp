
/*!
@file UseWepon.cpp
@brief UseWeponクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "UseWepon.h"

#include "WeponBase.h"

namespace basecross {

	UseWepon::UseWepon(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr)
	{}

	void UseWepon::OnUpdate() {
		
	}

}