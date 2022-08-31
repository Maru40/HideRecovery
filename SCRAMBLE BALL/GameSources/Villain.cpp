/*!
@file Villain.cpp
@brief Villain‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "Villain.h"

namespace basecross {
	namespace Enemy {

		Villain::Villain(const std::shared_ptr<GameObject>& objPtr) :
			EnemyBase(objPtr)
		{}

	}
}