/*!
@file VillainEnemyObject.cpp
@brief VillainEnemyObject�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "VillainEnemyObject.h"

#include "Maruyama/Enemy/Component/Villain.h"

#include "Maruyama/Enemy/StateMachine/EnemyMainStateMachine.h"
#include "Maruyama/Enemy/Component/Stator/Stator_Villain.h"

namespace basecross {
	namespace Enemy {

		VillainObject::VillainObject(const std::shared_ptr<Stage>& stage) :
			EnemyObjectBase(stage)
		{}

		void VillainObject::OnCreate() {
			EnemyObjectBase::OnCreate();

			SettingModel();

			AddComponent<Villain>();
			AddComponent<Stator_Villain>();
		}

		void VillainObject::SettingModel() {
			Mat4x4 spanMat;
			Vec3 scale = Vec3(0.6f);
			spanMat.affineTransformation(
				scale,
				Vec3(0.0f),
				Vec3(0.0f, XM_PI, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			);

			auto draw = AddComponent<DrawComp>();
			draw->SetMultiMeshResource(L"Player_Mesh");
			draw->SetMeshToTransformMatrix(spanMat);

			//draw->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 1.0f));

			transform->SetScale(Vec3(1.0f, 1.5f, 1.0f));
		}
	}
}