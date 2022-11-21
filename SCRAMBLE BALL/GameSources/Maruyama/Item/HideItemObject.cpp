/*!
@file HideItemObject.cpp
@brief HideItemObjectクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "HideItemObject.h"

#include "HideItem.h"
#include "Maruyama/Utility/Component/Targeted.h"
#include "Itabashi/ObjectHider.h"
#include "Itabashi/Item.h"
#include "Watanabe/Utility/DataExtracter.h"
#include "Watanabe/Component/BallAnimator.h"
#include "Watanabe/Effekseer/EfkEffect.h"
#include "Watanabe/Component/BallObjectEventExecuter.h"
#include "Watanabe/UI/DirectionWithHasBallUI.h"
#include "Watanabe/UI/UIObjectCSVBuilder.h"

#include "MainStage.h"
#include "Patch/PlayerInputer.h"

#include "Itabashi/OnlineStatus.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

#include "Maruyama/Enemy/Component/SelfAstarNodeController.h"

namespace basecross {
	HideItemObject::HideItemObject(const std::shared_ptr<Stage>& stage) :
		StageObjectBase(stage, L"HideItem")
	{}

	HideItemObject::HideItemObject(const std::shared_ptr<Stage>& stage, const std::wstring& line) :
		StageObjectBase(stage, L"HideItem")
	{
		vector<wstring> tokens = DataExtracter::DelimitData(line);
		size_t nextIndex = DataExtracter::TransformDataExtraction(tokens, m_transformData);
	}

	void HideItemObject::OnCreate() {
		SettingModel();

		AddComponent<Online::OnlineStatus>();

		AddComponent<Operator::ObjectHider>();

		AddComponent<Item>();

		AddComponent<HideItem>();
		AddComponent<SelfAstarNodeController>()->SetUpdateActive(false);

		auto collision = AddComponent<CollisionSphere>();
		collision->SetAfterCollision(AfterCollision::None);

		auto targeted = AddComponent<Targeted>(Targeted::Parametor(TargetedPriority::BALL));
		targeted->AddCanTargetFunction([&]() { return IsDrawActive(); });

		auto animator = AddComponent<BallAnimator>();
		animator->ChangeBallAnimation(BallAnimationState::State::Gwait);

		if (auto shareClass = ShareClassesManager::GetInstance()) {
			shareClass->AddShareClass<HideItemObject>(GetThis<HideItemObject>());
		}

		// メインステージのみ
		if (auto mainStage = GetTypeStage<MainStage>(false)) {
			// エフェクト
			auto efkComp = AddComponent<EfkComponent>();
			efkComp->SetEffectResource(L"HasBall", TransformData(Vec3(0), Vec3(0.5f)));

			auto eventExecuter = AddComponent<BallObjectEventExecuter>();
			// エフェクト
			eventExecuter->AddEvent(
				[efkComp]() {
					efkComp->PlayLoop(L"HasBall");
				},
				[efkComp]() {
					efkComp->Stop(L"HasBall");
				}
				);

			auto directionUI = mainStage->GetUIObjectCSVBuilder()
				->GetUIObject<DirectionWithHasBallUI>(L"DirectionWithHasBallUI");

			// 方向指示UI
			eventExecuter->AddEvent(
				[&, directionUI]() {
					directionUI->SetTarget(GetThis<HideItemObject>());
				},
				[directionUI]() {
					directionUI->ClearTarget();
				}
				);
		}
	}

	void HideItemObject::OnUpdate() {
		//デバッグ-----------------------------------------------------------------

		//auto animator = GetComponent<BallAnimator>(false);

		//if (PlayerInputer::GetInstance()->IsLeftDown()) {
		//	animator->ChangeBallAnimation(BallAnimationState::State::Goal);
		//}

		//-------------------------------------------------------------------------
	}

	void HideItemObject::SettingModel() {
		Mat4x4 spanMat;
		const float fScale = 1.0f;
		Vec3 scale = Vec3(fScale);
		spanMat.affineTransformation(
			scale,
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, XM_PI, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		auto draw = AddComponent<DrawComp>();
		draw->SetMultiMeshResource(L"Ball_Model");
		//draw->SetDiffuse(Col4(0.0f, 0.5f, 0.0f, 1.0f));
		draw->SetMeshToTransformMatrix(spanMat);

		// アウトライン関係
		draw->SetOutlineActive(true);
		draw->SetOutlineWidth(0.05f);

		transform->SetScale(Vec3(0.5f));
	}
}