/*!
@file StateNode_Camera_TPS.cpp
@brief StateNode_Camera_TPS�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "StateNodeBase.h"
#include "StateNode_Camera_TPS.h"

#include "GameStageBase.h"
#include "MainCameraObject.h"
#include "SeekTPS.h"
#include "SeekTarget.h"
#include "LookAtCameraManager.h"
#include "TargetMarkerManager.h"

#include "PlayerInputer.h"
#include "ChangeCameraManager.h"
#include "LookTargetManager.h"

namespace basecross {

	namespace StateNode
	{
		Camera_TPS::Camera_TPS(const std::shared_ptr<GameObject>& objPtr)
			:StateNodeBase(objPtr)
		{}


		void Camera_TPS::OnStart() {
			//�v���C���[���ʂ̕��@�Ŏ擾�ł��Ȃ�������
			auto stage = GetOwner()->GetGameStage();
			if (!stage) {
				return;
			}

			auto mainCameraObject = stage->GetMainCameraObject();
			if (!mainCameraObject) {
				return;
			}

			auto player = stage->GetPlayer();
			auto seekTPS = mainCameraObject->GetComponent<SeekTPS>(false);
			auto seekTarget = mainCameraObject->GetComponent<SeekTarget>(false);
			auto lookAt = mainCameraObject->GetComponent<LookAtCameraManager>(false);
			auto targetMarkerManager = player->GetComponent<TargetMarkerManager>(false);

			if (seekTPS) {
				seekTPS->SetUpdateActive(true);
			}
			if (seekTarget) {
				seekTarget->SetUpdateActive(false);
			}
			if (lookAt) {
				lookAt->SetTarget(player);
			}
			if (targetMarkerManager) {
				targetMarkerManager->SetMarkerActive(false);
				targetMarkerManager->SetTarget(nullptr);
			}

			stage->ChangeMainCamera();

			//LookTarget��nullptr�ɕύX
			auto lookTargetManager = player->GetComponent<LookTargetManager>(false);
			if (lookTargetManager) {
				lookTargetManager->SetTarget(nullptr);
			}
		}

		bool Camera_TPS::OnUpdate() {
			if (PlayerInputer::IsChangeTargetLookCamera()) { //����
				ChangeCameraManager::GetInstance()->ChangeState(ChangeCameraManager::State::Look);
			}

			return false;
		}

		void Camera_TPS::OnExit() {

		}

	}
}