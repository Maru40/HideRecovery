
/*!
@file ItemBase.cpp
@brief ItemBaseクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "HideItem.h"

namespace basecross {

	ItemBase::ItemBase(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr), m_isAcquisition(false)
	{}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void ItemBase::SetIsAcquisition(const bool isAcquisition) {
		m_isAcquisition = isAcquisition;
	}

}