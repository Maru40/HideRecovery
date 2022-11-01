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
#include "Watanabe/Component/PlayerAnimator.h"

#include "Maruyama/Enemy/Component/Stator/AIPlayerStator.h"

namespace basecross {
	namespace Enemy {

		AIPlayerObject::AIPlayerObject(const std::shared_ptr<Stage>& stage):
			EnemyObjectBase(stage)
		{}

		void AIPlayerObject::OnCreate() {
			EnemyObjectBase::OnCreate();

			AddComponent<PlayerStatus>();
			AddComponent<PlayerAnimator>();

			AddComponent<AIPlayerStator>();
			//AddComponent<AIPlayerBehaviorTree>();
		}

		void AIPlayerObject::CreateModel() {
			Mat4x4 spanMat;
			const float fScale = 0.8f;
			Vec3 scale = Vec3(fScale);
			spanMat.affineTransformation(
				scale,
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, XM_PI, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);

			auto draw = AddComponent<DrawComp>();
			draw->SetMultiMeshResource(L"Player_Mesh");
			draw->SetMeshToTransformMatrix(spanMat);
			draw->SetModelTextureEnabled(false);
			SetAlphaActive(true);
		}

	}
}