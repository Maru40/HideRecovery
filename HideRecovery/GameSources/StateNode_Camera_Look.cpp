/*!
@file StateNode_Camera_Look.cpp
@brief StateNode_Camera_Lookなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "StateNodeBase.h"
#include "StateNode_Camera_Look.h"

#include "GameStageBase.h"
#include "MainCameraObject.h"
#include "SeekTPS.h"
#include "SeekTarget.h"
#include "LookAtCameraManager.h"
#include "LookTargetManager.h"
#include "TargetMarkerManager.h"
#include "ChangeCameraManager.h"

#include "PlayerInputer.h"

namespace basecross {

	namespace StateNode
	{

		Camera_Look::Camera_Look(const std::shared_ptr<GameObject>& objPtr)
			:StateNodeBase(objPtr)
		{}


		void Camera_Look::OnStart() {
			//プレイヤーが別の方法で取得できないか検討
			auto stage = GetOwner()->GetGameStage();
			if (!stage) {
				return;
			}
			auto mainCameraObject = stage->GetMainCameraObject();

			auto player = stage->GetPlayer();
			auto seekTPS = mainCameraObject->GetComponent<SeekTPS>(false);
			auto seekTarget = mainCameraObject->GetComponent<SeekTarget>(false);
			auto lookAt = mainCameraObject->GetComponent<LookAtCameraManager>(false);
			auto lookTargetManager = player->GetComponent<LookTargetManager>(false);
			auto targetMarkerManager = player->GetComponent<TargetMarkerManager>(false);
			//コンポーネントがあるなら
			if (lookTargetManager) { 
				lookTargetManager->SearchTarget();
			}

			if (seekTPS) {
				seekTPS->SetUpdateActive(false);
			}
			if (seekTarget) {
				seekTarget->SetUpdateActive(true);
			}
			if (lookTargetManager) {
				if (lookAt) {
					lookAt->SetTarget(lookTargetManager->GetTarget());
				}
			}
			if (targetMarkerManager) {
				targetMarkerManager->SetTarget(lookTargetManager->GetTarget());
				targetMarkerManager->SetMarkerActive(true);
			}

			stage->ChangeMainCamera();
		}

		bool Camera_Look::OnUpdate() {
			InputManager();

			return false;
		}

		void Camera_Look::OnExit() {
			//プレイヤーが別の方法で取得できないか検討
			auto stage = GetOwner()->GetGameStage();
			if (!stage) {
				return;
			}
			auto mainCameraObject = stage->GetMainCameraObject();

			auto player = stage->GetPlayer();
			auto lookAt = mainCameraObject->GetComponent<LookAtCameraManager>(false);
			auto lookTargetManager = player->GetComponent<LookTargetManager>(false);
			auto targetMarkerManager = player->GetComponent<TargetMarkerManager>(false);

			if (lookTargetManager) {
				if (lookAt) {
					lookAt->SetTarget(nullptr);
				}
			}
			if (targetMarkerManager) {
				targetMarkerManager->SetMarkerActive(false);
				targetMarkerManager->SetTarget(nullptr);
			}
		}

		void Camera_Look::InputManager() {
			if (PlayerInputer::IsChangeTargetLookCamera()) { //入力
				ChangeCameraManager::GetInstance()->ChangeState(ChangeCameraManager::State::TPS);
			}
		}

	}
}