#include "stdafx.h"
#include "DirectionWithHasBallUI.h"
#include "../Manager/SpriteDataManager.h"
#include "../Utility/Utility.h"

#include "../DebugClass/Debug.h"

namespace basecross {
	DirectionWithHasBallUI::DirectionWithHasBallUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"DirectionWithHasBallUI")
	{}

	shared_ptr<SimpleSprite> DirectionWithHasBallUI::CreateSprite(const wstring& spriteKey) {
		auto sprite = GetStage()->AddGameObject<SimpleSprite>(
			SimpleSprite::Type::SpriteData, spriteKey
			);
		return sprite;
	}

	void DirectionWithHasBallUI::OnCreate() {
		m_camera = GetStage()->GetView()->GetTargetCamera();

		m_cursor = CreateSprite(L"Triangle");
		auto rectTrans = m_cursor->GetRectTransform();
		rectTrans->SetPosition(Vec2(0, 400));
		rectTrans->SetScale(0.3f, -0.3f);
		m_cursor->SetParent(GetThis<DirectionWithHasBallUI>());
	}
	void DirectionWithHasBallUI::OnUpdate() {
		Vec3 cameraForward = m_camera->GetAt() - m_camera->GetEye();
		cameraForward.normalize();
		//
		auto vec = Utility::Vector3ProjectOnPlane(Vec3(0, 0, 0) - m_camera->GetAt(), cameraForward);
		vec.normalize();
		auto cameraUp = m_camera->GetUp().GetNormalized();
		//auto cameraUp = Vec3(1, 0, 0);
		// カメラの上方向（スクリーンの上方向）と
		// スクリーン上のベクトル（vec）の角度を求める
		auto angle = Utility::GetTwoVectorAngle360(cameraUp, vec, cameraForward);
		Debug::GetInstance()->Log(360 - angle);
		Debug::GetInstance()->Log(angle);

		GetRectTransform()->SetRotation(XMConvertToRadians(angle));
	}
}