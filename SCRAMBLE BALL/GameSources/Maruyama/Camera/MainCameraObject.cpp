/*!
@file MainCameraObj.cpp
@brief MainCameraObjのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Camera/Component/SeekTPS.h"
#include "MainCameraObject.h"
#include "Maruyama/Utility/Component/ToTargetMove.h"
#include "Maruyama/Camera/Component/LookAtCameraManager.h"
#include "Maruyama/Utility/Component/SeekTarget.h"

#include "Maruyama/Utility/Timer/GameTimer.h"
#include "Maruyama/Camera/Component/CameraShake.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

#include "GameStageBase.h"

#include "PlayerInputer.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Utility/UtilityObstacle.h"
#include "Maruyama/Utility/MaruHitHelper.h"
#include "Maruyama/Utility/Mathf.h"

#include "Maruyama/Camera/Component/CameraRayAlphaManager.h"

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

	//アクセッサ---------------------------------------------------------------------------------

	void MainCameraObject::SetTarget(const std::shared_ptr<GameObject>& target) {
		m_target = target;

		auto seek = GetComponent<SeekTPS>();
		seek->SetTarget(target);
	}
}

//endbasecross