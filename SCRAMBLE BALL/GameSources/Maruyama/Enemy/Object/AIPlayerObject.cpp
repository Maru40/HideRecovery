/*!
@file AIPlayerObject.cpp
@brief AIPlayerObject‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyObjectBase.h"
#include "AIPlayerObject.h"

#include "Maruyama/Player/Component/ItemBag.h"
#include "Maruyama/Utility/Component/RotationController.h"
#include "VelocityManager.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Utility/Component/SeekTarget.h"

#include "Maruyama/Enemy/Component/EyeSearchRange.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"
#include "Maruyama/Enemy/Component/BehaviorTree/AIPlayerBehaviorTree.h"

#include "Watanabe/Component/PlayerStatus.h"

namespace basecross {
	namespace Enemy {

		AIPlayerObject::AIPlayerObject(const std::shared_ptr<Stage>& stage):
			EnemyObjectBase(stage)
		{}

		void AIPlayerObject::OnCreate() {
			EnemyObjectBase::OnCreate();

			AddComponent<PlayerStatus>();
			AddComponent<AIPlayerBehaviorTree>();
		}

	}
}