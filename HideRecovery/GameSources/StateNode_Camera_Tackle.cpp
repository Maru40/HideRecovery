/*!
@file StateNode_Camera_Tackle.cpp
@brief StateNode_Camera_Tackle�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "StateNodeBase.h"
#include "StateNode_Camera_Tackle.h"

#include "GameStageBase.h"
#include "TackleCameraObject.h"
#include "SeekTPS.h"
#include "SeekTarget.h"
#include "LookAtCameraManager.h"

#include "PressData.h"
#include "SlowTimerManager.h"
#include "TimeHelper.h"
#include "GameTimer.h"

#include "HitStopManager.h"

#include "TargetCameraManager.h"
#include "TargetMarkerManager.h"

#include "GameStageBase.h"

namespace basecross {

	namespace StateNode
	{
		Camera_Tackle::Camera_Tackle(const std::shared_ptr<GameObject>& objPtr)
			:StateNodeBase(objPtr), m_timer(new GameTimer(0.0f))
		{ }

		void Camera_Tackle::OnStart() {		
			//Player��ʂ̏ꏊ�ŋ��L�ł���悤�Ɍ���
			auto stage = GetOwner()->GetGameStage();
			auto tacleCameraObject = stage->GetTackleCameraObject();
			auto lookAt = tacleCameraObject->GetComponent<LookAtCameraManager>(false);
			auto tackleCameraMangaer = maru::Utility::FindComponent<TargetCameraManager>();
			auto targetMarkerManager = stage->GetPlayer()->GetComponent<TargetMarkerManager>(false);

			//�^�b�N���̃^�C�}�[�Z�b�g
			if (SlowTimerManager::GetInstance()->IsSlow()) { //SlowTimer�Ȃ�
				m_timer->ResetTimer(SlowTimerManager::GetInstance()->GetSlowTimeParametor().time);
			}
			else { //HitStopTimer�Ȃ�
				m_timer->ResetTimer(HitStopManager::GetInstance()->GetStopData().startTime,
					[&, this]() { m_timer->ResetTimer(HitStopManager::GetInstance()->GetStopData().stopTime); });
			}

			if (lookAt && targetMarkerManager) { 
				lookAt->SetTarget(targetMarkerManager->GetTarget());
			}
			if (tackleCameraMangaer) { //�^�b�N���̃X�^�[�g
				tackleCameraMangaer->StartTask();
			}

			if (auto tackleCameraObject = stage->GetTackleCameraObject()) {
				tackleCameraObject->SetUpdateActive(true);
			}

			stage->ChangeTackleCamera();

		}

		bool Camera_Tackle::OnUpdate() {
			m_timer->UpdateTimer(1.0f, maru::DeltaType::Real);
			return m_timer->IsTimeUp();
		}

		void Camera_Tackle::OnExit() {
			auto stage = GetOwner()->GetGameStage();

			if (auto tackleCameraObject = stage->GetTackleCameraObject()) {
				tackleCameraObject->SetUpdateActive(false);
			}
		}

	}
}