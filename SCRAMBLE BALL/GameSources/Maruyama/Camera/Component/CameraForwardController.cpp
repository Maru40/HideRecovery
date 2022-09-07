/*!
@file CameraForwardController.cpp
@brief CameraForwardControllerなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "CameraForwardController.h"

#include "Patch/CameraHelper.h"

namespace basecross {

	CameraForwardController::CameraForwardController(
		const std::shared_ptr<GameObject>& objPtr,
		const std::shared_ptr<VirtualCamera>& virtualCamera
	) :
		Component(objPtr),
		m_virtualCamera(virtualCamera)
	{}

	void CameraForwardController::OnCreate() {

	}

	void CameraForwardController::OnUpdate2() {
		auto virtualCamera = m_virtualCamera.lock();
		if (!virtualCamera) {
			return;
		}

		if (!virtualCamera->IsUpdateActive()) {	//VirtualCameraが非アクティブなら処理をしない。
			return;
		}
		
		ForwardUpdate();
	}

	void CameraForwardController::ForwardUpdate() {
		auto camera = GetStage()->GetView()->GetTargetCamera();
		if (!camera) {
			return;
		}

		auto position = camera->GetEye();
		auto atPosition = position + m_direction;

		camera->SetAt(atPosition);
	}

}