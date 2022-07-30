/*!
@file MainCamera.cpp
@brief MainCameraのクラス実体
担当：丸山裕喜
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

		//カメラオブジェクトが存在したら。
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