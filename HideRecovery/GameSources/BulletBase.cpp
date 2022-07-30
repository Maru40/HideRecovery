
/*!
@file StatorBase.cpp
@brief StatorBase�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "BulletBase.h"

namespace basecross {

	BulletBase::BulletBase(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr)
	{}

}