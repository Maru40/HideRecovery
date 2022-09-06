/*!
@file EnemyObjectBase.cpp
@brief EnemyObjectBase�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyObjectBase.h"

#include "Maruyama/Player/Component/ItemBag.h"
#include "Maruyama/Utility/Component/RotationController.h"
#include "VelocityManager.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Utility/Component/SeekTarget.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"

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