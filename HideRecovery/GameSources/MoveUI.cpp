/*!
@file ParticleManager.cpp
@brief ParticleManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "MoveUI.h"

#include "PlayerInputer.h"

#include "DebugObject.h"

namespace basecross {

	MoveUI::MoveUI(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr), m_speed(100.0f)
	{}

	void MoveUI::OnUpdate() {
		float delta = App::GetApp()->GetElapsedTime();
		auto direct = PlayerInputer::GetMoveDirection();

		float speed = PlayerInputer::IsYDown() ? m_speed * 0.5f : m_speed;

		auto position = transform->GetPosition();
		position += Vec3(direct.x, direct.y, 0.0f) * speed * delta;
		transform->SetPosition(position);

		if (PlayerInputer::IsXDown()) {
			DebugObject::AddString(L"UIPosition: ", false);
			DebugObject::AddVector(transform->GetPosition());
		}
	}


}