/*!
@file HeroObject.cpp
@brief HeroObject�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "HeroObject.h"

#include "Maruyama/Enemy/Component/Hero.h"

#include "Maruyama/Enemy/StateMachine/EnemyMainStateMachine.h"
#include "Maruyama/Enemy/Component/Stator/Stator_Hero.h"

#include "Maruyama/Enemy/ImpactMap/AIImpacter.h"
#include "Maruyama/Enemy/ImpactMap/SelfImpactNodeManager.h"

#include "Watanabe/Component/PlayerStatus.h"

namespace basecross {
	namespace Enemy {

		HeroObject::HeroObject(const std::shared_ptr<Stage>& stage):
			EnemyObjectBase(stage)
		{}

		void HeroObject::OnCreate() {
			EnemyObjectBase::OnCreate();

			SettingModel();

			AddComponent<Hero>();
			AddComponent<Stator_Hero>();

			auto astarComp = AddComponent<AIImpacter>();
			AddComponent<maru::SelfImpactNodeManager>(astarComp);

			AddComponent<PlayerStatus>();
		}

		void HeroObject::SettingModel() {
			//Mat4x4 spanMat;
			//Vec3 scale = Vec3(0.5f);
			//spanMat.affineTransformation(
			//	scale,
			//	Vec3(0.0f),
			//	Vec3(0.0f, XM_PI, 0.0f),
			//	Vec3(0.0f, -0.5f, 0.0f)
			//);

			Mat4x4 spanMat;
			const float fScale = 0.6f;
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

			draw->SetDiffuse(Col4(1.0f, 0.0f, 0.0f, 1.0f));

			transform->SetScale(Vec3(1.0f, 1.5f, 1.0f));
		}
	}
}