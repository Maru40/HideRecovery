/*!
@file TargetCameraManager.cpp
@brief TargetCameraManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TargetCameraManager.h"

#include "MulchTaskList.h"

#include "Task_CameraAtMove.h"
#include "Task_ToTargetMove.h"

#include "GameStageBase.h"
#include "TackleCamera.h"
#include "TackleCameraObject.h"

#include "MaruUtility.h"

#include "ChangeCameraManager.h"

namespace basecross {

	TargetCameraManager::TargetCameraManager(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr), m_mulchTaskList(new MulchTaskList<TaskEnum>())
	{}
	
	void TargetCameraManager::OnCreate() {
		DefineTask();
	}

	void TargetCameraManager::OnUpdate() {
		if (m_mulchTaskList->IsEnd()) {
			return;
		}

		m_mulchTaskList->UpdateTask();

		auto changeCameraManager = maru::Utility::FindComponent<ChangeCameraManager>();
		if (changeCameraManager) {
			if (changeCameraManager->GetNowState() != ChangeCameraManager::State::Tackle) {
				m_mulchTaskList->ForceReset();
			}
		}
	}

	void TargetCameraManager::DefineTask() {
		auto stage = GetGameStage();
		auto cameraObject = stage->GetTackleCameraObject();
		auto camera = stage->GetTackleCamera();

		//視界の制御
		auto atParam = Task_CameraAtMove::Parametor();
		m_mulchTaskList->DefineTask(TaskEnum::AtMove, std::make_shared<Task_CameraAtMove>(camera, atParam));

		//場所の制御
		m_eyeParamPtr = std::make_shared<Task_ToTargetMove::Parametor>();
		m_mulchTaskList->DefineTask(TaskEnum::EyeMove, std::make_shared<Task_ToTargetMove>(cameraObject, m_eyeParamPtr));
	}

	void TargetCameraManager::SelectTask() {
		vector<TaskEnum> tasks = {
			TaskEnum::AtMove,
			TaskEnum::EyeMove,
		};

		auto player = GetGameStage()->GetPlayer();
		auto playerTrans = player->GetComponent<Transform>();
		auto playerPosition = playerTrans->GetPosition();
		auto pusher = player->GetComponent<Pusher>(false);
		auto pressData = pusher->GetSuccessPressData();
		auto forward = -pressData.collisionPair.m_SrcHitNormal;	

		//視界パラメータ
		auto atTask = m_mulchTaskList->GetDefineTask<Task_CameraAtMove>(TaskEnum::AtMove);
		auto atParam = atTask->GetParametors()[0];
		atParam.startPosition = playerPosition + maru::Utility::ConvertForwardOffset(forward, Vec3(2.0f, 0.0f, -2.0f));    //開始場所 
		atParam.endPosition = playerPosition + maru::Utility::ConvertForwardOffset(forward, Vec3(3.0f, 0.0f, 0.0f));       //終了場所
		atParam.speed = 3.0f;
		atParam.moveType = Task_CameraAtMove::MoveType::Lerp;
		atParam.deltaType = Task_CameraAtMove::DeltaType::Real;
		atTask->SetParametors(vector<Task_CameraAtMove::Parametor>(1, atParam));
		
		//場所パラメータ
		auto eyeTask = m_mulchTaskList->GetDefineTask<Task_ToTargetMove>(TaskEnum::EyeMove);
		auto eyeParam = eyeTask->GetParametor();
		eyeParam->startPosition = playerPosition + maru::Utility::ConvertForwardOffset(forward, Vec3(11.0f, 7.0f, -5.0f)); //開始場所
		eyeParam->endPosition = playerPosition + maru::Utility::ConvertForwardOffset(forward, Vec3(11.0f, 7.0f, -1.0f));   //終了場所
		eyeParam->speed = 4.0f;
		eyeParam->moveType = Task_ToTargetMove::MoveType::Lerp;
		eyeParam->deltaType = Task_ToTargetMove::DeltaType::Real;
		eyeTask->SetParametor(eyeParam);

		m_mulchTaskList->AddTask(tasks);
	}

	void TargetCameraManager::StartTask() {
		SelectTask();
	}
}

//endbasecross