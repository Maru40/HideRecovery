/*!
@file Hero.cpp
@brief Hero‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "Hero.h"

namespace basecross {
	namespace Enemy {

		Hero::Hero(const std::shared_ptr<GameObject>& objPtr) :
			EnemyBase(objPtr)
		{}

	}
}