/*!
@file Villain.h
@brief Villain�Ȃ�
�S���ҁF�ێR �T��
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