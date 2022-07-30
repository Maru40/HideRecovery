/*!
@file TackleCameraObject.cpp
@brief TackleCameraObjectのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TackleCameraObject.h"

#include "SeekTarget.h"
#include "LookAtCameraManager.h"
#include "CameraShake.h"

#include "CameraRayAlphaManager.h"

#include "GameStageBase.h"

namespace basecross {

	TackleCameraObject::TackleCameraObject(const std::shared_ptr<Stage>& stage, const std::shared_ptr<GameObject>& seekTarget)
		:GameObject(stage), m_seekTarget(seekTarget)
	{ }

	void TackleCameraObject::OnCreate() {
		AddComponent<CameraShake>();
		const float RayRange = 4.0f;
		auto rayAlpha = AddComponent<CameraRayAlphaManager>(GetGameStage()->GetPlayer(), RayRange);
		SetUpdateActive(false);
	}

	void TackleCameraObject::SettingSeek(const std::shared_ptr<GameObject>& target) {
		auto param = SeekTarget::Parametor();
		param.seekType = SeekTarget::SeekType::Normal;
		param.offset = Vec3(2.0f, 0.0f, 1.0f);

		AddComponent<SeekTarget>(target, param);
	}

	void TackleCameraObject::SettingLookAt(const std::shared_ptr<GameObject>& target) {
		auto param = LookAtCameraManager::Parametor();
		param.centerOffset = Vec3(0.0f, 0.0f , 0.0f);

		AddComponent<LookAtCameraManager>(target, param);
	}

	//アクセッサ---------------------------------------------------------------------------------

	void TackleCameraObject::SetSeekTarget(const std::shared_ptr<GameObject>& target) noexcept {
		GetComponent<SeekTarget>()->SetTarget(target);
	}

	void TackleCameraObject::SetLockAtTarget(const std::shared_ptr<GameObject>& target) noexcept {
		GetComponent<LookAtCameraManager>()->SetTarget(target);
	}
}

//endbasecross