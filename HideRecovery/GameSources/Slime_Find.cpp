
/*!
@file Slime_Find.cpp
@brief Slime_Find�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyBase.h"
#include "TargetManager.h"
#include "Slime_Find.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Slime_Find::Slime_Find(const std::shared_ptr<EnemyBase>& owner)
				:EnemyStateNodeBase<EnemyBase>(owner)
			{}

		}
	}
}