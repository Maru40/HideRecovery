
/*!
@file ItemBase.cpp
@brief ItemBase�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "HideItem.h"

namespace basecross {

	ItemBase::ItemBase(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr), m_isAcquisition(false)
	{}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void ItemBase::SetIsAcquisition(const bool isAcquisition) {
		m_isAcquisition = isAcquisition;
	}

}