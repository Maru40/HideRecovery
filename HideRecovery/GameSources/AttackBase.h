
/*!
@file AttackBase.h
@brief AttackBase‚È‚Ç
’S“–FŠÛR—TŠì
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// UŒ‚‚ÌŠî’êƒNƒ‰ƒX
	//--------------------------------------------------------------------------------------
	class AttackBase : public Component
	{
	public:
		AttackBase(const std::shared_ptr<GameObject>& objPtr) :
			Component(objPtr)
		{}
	};

}