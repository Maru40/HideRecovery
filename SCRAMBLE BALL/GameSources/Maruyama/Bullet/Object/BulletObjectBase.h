
/*!
@file BulletObjectBase.h
@brief BulletObjectBase‚È‚Ç
’S“–FŠÛR—TŠì
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class BulletObjectBase : public GameObject
	{
	public:
		BulletObjectBase(const std::shared_ptr<Stage>& stage) :
			GameObject(stage)
		{}
	};

}