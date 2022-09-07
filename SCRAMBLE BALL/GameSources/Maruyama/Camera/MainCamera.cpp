/*!
@file MainCamera.cpp
@brief MainCamera�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "MainCamera.h"
#include "MainCameraObject.h"

namespace basecross {

	MainCamera::MainCamera(const std::weak_ptr<GameObject>& target) :
		Camera(), m_target(target)
	{}

	void MainCamera::SetTarget(const std::shared_ptr<GameObject>& target) {
		m_target = target;

		//�J�����I�u�W�F�N�g�����݂�����B
		auto cameraObject = GetCameraObject();
		if (cameraObject) {
			auto mainCameraObject = dynamic_pointer_cast<MainCameraObject>(cameraObject);
			if (mainCameraObject) {
				mainCameraObject->SetTarget(target);
			}
		}
	}

}

//endbasecross