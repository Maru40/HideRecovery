/*!
@file AIPlayerObject.cpp
@brief AIPlayerObject�Ȃǎ���
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

//Oline�ɕK�v�ȃR���|�[�l���g-------------------------------------------
#include "Maruyama/Player/Component/ChargeGun.h"

#include "Maruyama/Player/Component/Teleport.h"
#include "Maruyama/Player/Component/ItemAcquisitionManager.h"
#include "Maruyama/Player/Component/UseWeapon.h"
#include "Maruyama/Player/Component/GoalAnimationController.h"
#include "Maruyama/Player/Component/HidePlaceOpener.h"
#include "Itabashi/ObjectMover.h"
#include "Itabashi/PlayerControlManager.h"
#include "Itabashi/OnlinePlayerSynchronizer.h"
#include "Itabashi/OnlineTransformSynchronization.h"
#include "Maruyama/Player/Component/Respawner.h"
#include "Maruyama/Player/Component/PlayerDeader.h"
//-----------------------------------------------------------------------

//#include "Watanabe/DebugClass/Debug.h"
//#include "Watanabe/UI/UIObjects.h"
//#include "Watanabe/Effekseer/EfkEffect.h"

namespace basecross {
	namespace Enemy {

		AIPlayerObject::AIPlayerObject(const std::shared_ptr<Stage>& stage):
			EnemyObjectBase(stage)
		{}

		void AIPlayerObject::OnCreate() {
			EnemyObjectBase::OnCreate();

			AddComponent<PlayerStatus>();
			AddComponent<PlayerAnimator>();

			//Oline�ɕK�v�ȃR���|�[�l���g------------------------
			auto chargeGun = AddComponent<ChargeGun>();
			auto soundEmitter = AddComponent<SoundEmitter>();

			AddComponent<Teleport>();
			AddComponent<ItemAcquisitionManager>();
			AddComponent<UseWeapon>(chargeGun)->SetIsUpdateRotation(false);
			AddComponent<GoalAnimationController>();
			AddComponent<HidePlaceOpener>();
			AddComponent<Operator::ObjectMover>();
			AddComponent<PlayerControlManager>();
			AddComponent<Online::OnlineTransformSynchronization>();
			AddComponent<OnlinePlayerSynchronizer>();
			AddComponent<Respawner>();
			//---------------------------------------------------

			AddComponent<PlayerDeader>();

			//auto efkComp = AddComponent<EfkComponent>();



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