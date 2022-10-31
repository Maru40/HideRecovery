#include "stdafx.h"
#include "CameraTarget.h"
#include "../DebugClass/Debug.h"

namespace basecross {
	void CameraTarget::OnCreate() {
		AddComponent<CollisionObb>()->SetDrawActive(true);
	}
	void CameraTarget::OnUpdate() {
		auto trans = GetComponent<Transform>();
		auto pos = trans->GetPosition();

		auto dir = m_camera->GetAt() - m_camera->GetEye();
		dir.y = 0.0f;
		dir.normalize();

		Vec3 Horizontal = dir;
		Vec3 Vertical(Horizontal.z, 0, -Horizontal.x);

		Vec3 moveH = Vec3(Horizontal * m_controller.GetLeftStickVec().z);
		Vec3 moveV = Vec3(Vertical * m_controller.GetLeftStickVec().x);
		Vec3 moveVec = moveH + moveV;

		pos += moveVec * 10 * App::GetApp()->GetElapsedTime();
		trans->SetPosition(pos);
	}
}