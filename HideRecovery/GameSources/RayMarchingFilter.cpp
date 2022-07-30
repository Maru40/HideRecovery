/*!
@file RayMarchingFilterObject.cpp
@brief RayMarchingFilterObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "RayMarchingFilter.h"
#include "BcPNTSlimeDraw.h"

#include "SeekTarget.h"

#include "MainCamera.h"
#include "GameStageBase.h"

namespace basecross {

	RayMarchingFilterObject::RayMarchingFilterObject(const std::shared_ptr<Stage>& stage)
		:GameObject(stage), m_target(nullptr)
	{}

	void RayMarchingFilterObject::OnCreate() {
		auto draw =  AddComponent<BcPNTSlimeDraw>();
		draw->SetDepthStencilState(DepthStencilState::None);
		auto defaultScale = Vec3(5.0f, 5.0f, 0.1f);
		transform->SetScale(defaultScale);
		
		auto stage = GetGameStage();
		auto camera = stage->GetCamera<Camera>();
		m_target = camera;

		DebugDraw();
	}

	void RayMarchingFilterObject::OnUpdate() {
		if (!m_target) {
			return;
		}

		m_target = GetGameStage()->GetCamera<Camera>();
		auto targetPosition = m_target->GetEye();
		auto targetForward =  m_target->GetAt() - targetPosition;

		const float Range = 2.0f;
		auto setPosition = targetPosition + targetForward.GetNormalized() * Range;
		transform->SetPosition(setPosition);
		transform->SetForward(targetForward.GetNormalized());
	}

	void RayMarchingFilterObject::DebugDraw() {
		auto drawComponent = AddComponent<BcPNTSlimeDraw>();
		drawComponent->SetMeshResource(L"DEFAULT_CUBE");
		drawComponent->SetDepthStencilState(DepthStencilState::Read);

		SetAlphaActive(true);
	}

	void RayMarchingFilterObject::SettingSeek() {
		//SeekTarget
		auto stage = GetGameStage();
		auto camera = stage->GetMainCameraObject();
		auto param = SeekTarget::Parametor();
		param.seekType = SeekTarget::SeekType::Normal;
		param.offset = Vec3(0);
		param.range = 2.0f;
		param.speed = 0.0f;
		AddComponent<SeekTarget>(nullptr, param);
	}
}

//endbasecross