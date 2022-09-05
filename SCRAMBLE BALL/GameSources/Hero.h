/*!
@file Hero.h
@brief Hero�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

namespace basecross {
	namespace Enemy {

		class Hero : public EnemyBase
		{
		public:
			Hero(const std::shared_ptr<GameObject>& objPtr);
		};

	}
}