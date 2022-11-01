#pragma once
#include "stdafx.h"
#include "../Utility/GameController.h"

namespace basecross {
	class CameraTarget :public GameObject {
		GameController m_controller;
		shared_ptr<Camera> m_camera;
	public:
		CameraTarget(const shared_ptr<Stage>& stage, const shared_ptr<Camera>& camera)
			:GameObject(stage), m_controller(0), m_camera(camera)
		{}

		void OnCreate()override;
		void OnUpdate()override;
	};
}
