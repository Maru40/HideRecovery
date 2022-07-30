/*!
@file MainCameraObj.cpp
@brief MainCameraObj�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "SeekTPS.h"
#include "MainCameraObject.h"
#include "ToTargetMove.h"
#include "LookAtCameraManager.h"
#include "SeekTarget.h"

#include "TimeHelper.h"
#include "GameTimer.h"
#include "CameraShake.h"

#include "TimeHelper.h"
#include "GameTimer.h"

#include "GameStageBase.h"

#include "PlayerInputer.h"

#include "MaruUtility.h"
#include "UtilityObstacle.h"
#include "MaruHitHelper.h"
#include "Mathf.h"

#include "CameraRayAlphaManager.h"

#include "DebugObject.h"

namespace basecross {

	MainCameraObject::MainCameraObject(const std::shared_ptr<Stage>& stage, const std::shared_ptr<GameObject>& target) :
		GameObject(stage),
		m_target(target)
	{}

	void MainCameraObject::OnCreate()
	{
		AddComponent<SeekTPS>(m_target);
		AddComponent<SeekTarget>(m_target)->SetUpdateActive(false);
		AddComponent<ToTargetMove>();
		AddComponent<LookAtCameraManager>(m_target);
		AddComponent<CameraShake>();

		auto rayAlpha = AddComponent<CameraRayAlphaManager>(m_target);
	}

	//�A�N�Z�b�T---------------------------------------------------------------------------------

	void MainCameraObject::SetTarget(const std::shared_ptr<GameObject>& target) {
		m_target = target;

		auto seek = GetComponent<SeekTPS>();
		seek->SetTarget(target);
	}
}

//endbasecross