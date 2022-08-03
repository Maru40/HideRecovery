
/*!
@file PlayerObject.cpp
@brief PlayerObject�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "PlayerObject.h"

#include "PlayerMover.h"
#include "RotationController.h"
#include "PlayerController.h"

namespace basecross {

	PlayerObject::PlayerObject(const std::shared_ptr<Stage>& stage):
		GameObject(stage)
	{}

	void PlayerObject::OnCreate() {
		//�f�o�b�O
		auto draw = AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");

		transform->SetScale(Vec3(1.0f, 1.5f, 1.0f));

		AddComponent<PlayerMover>();
		AddComponent<RotationController>();
		AddComponent<PlayerController>();
	}

}