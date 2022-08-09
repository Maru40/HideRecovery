
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

	bool ItemBase::IsAcquisition() const {
		if (GetGameObject()->IsActive()) {
			return m_isAcquisition;
		}

		return false;
	}

	void ItemBase::SetIsAcquisition(const bool isAcquisition) {
		m_isAcquisition = isAcquisition;
	}

}