/*!
@file Villain.h
@brief Villain‚È‚Ç
’S“–ÒFŠÛR —TŠì
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "EnemyBase.h"

namespace basecross {
	namespace Enemy {

		class Villain : public EnemyBase
		{
		public:
			Villain(const std::shared_ptr<GameObject>& objPtr);
		};

	}
}