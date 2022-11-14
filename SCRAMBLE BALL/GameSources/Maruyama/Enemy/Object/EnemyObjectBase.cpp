﻿/*!
@file EnemyObjectBase.cpp
@brief EnemyObjectBaseなど実体
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

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"
#include "Maruyama/Enemy/Component/BehaviorTree/AIPlayerBehaviorTree.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Enemy/Component/SelfAstarNodeController.h"

#include "Maruyama/Player/Component/HidePlaceOpener.h"

namespace basecross {
	namespace Enemy {

		EnemyObjectBase::EnemyObjectBase(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		void EnemyObjectBase::OnCreate() {
			AddTag(L"T_Enemy");

			CreateModel();

			AddComponent<ItemBag>();

			AddComponent<EnemyBase>();
			AddComponent<CollisionObb>();
			AddComponent<RotationController>();
			AddComponent<Gravity>();
			AddComponent<VelocityManager>();
			AddComponent<TargetManager>();
			AddComponent<EyeSearchRange>();
			AddComponent<SeekTarget>(nullptr);
			AddComponent<HidePlaceOpener>()->SetIsDrawUI(false);
			AddComponent<SelfAstarNodeController>();

			//AddComponent<AIPlayerBehaviorTree>();
		}

		void EnemyObjectBase::CreateModel() {
			auto draw = AddComponent<PNTStaticDraw>();
			draw->SetMeshResource(L"DEFAULT_CUBE");
		}

	}
}