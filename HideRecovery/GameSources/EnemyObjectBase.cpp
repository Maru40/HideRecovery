/*!
@file EnemyObjectBase.cpp
@brief EnemyObjectBase‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyObjectBase.h"

#include "ItemBag.h"
#include "RotationController.h"
#include "VelocityManager.h"
#include "TargetManager.h"
#include "SeekTarget.h"

#include "EyeSearchRange.h"

namespace basecross {
	namespace Enemy {

		EnemyObjectBase::EnemyObjectBase(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		void EnemyObjectBase::OnCreate() {
			AddTag(L"T_Enemy");

			AddComponent<ItemBag>();

			AddComponent<CollisionObb>();
			AddComponent<RotationController>();
			AddComponent<Gravity>();
			AddComponent<VelocityManager>();
			AddComponent<TargetManager>();
			AddComponent<EyeSearchRange>();
			AddComponent<SeekTarget>(nullptr);
		}

	}
}