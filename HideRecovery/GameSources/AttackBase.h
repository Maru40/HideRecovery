
/*!
@file AttackBase.h
@brief AttackBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �U���̊��N���X
	//--------------------------------------------------------------------------------------
	class AttackBase : public Component
	{
	public:
		AttackBase(const std::shared_ptr<GameObject>& objPtr) :
			Component(objPtr)
		{}
	};

}