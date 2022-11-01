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
		m_rectTrans = GetRectTransform();
	}

	void DirectionWithHasBallUI::OnUpdate() {
		if (!m_targetObject) {
			//return;
		}

		auto view = GetStage()->GetView();
		// Viewportを0基準からー～＋範囲に変換
		const Viewport& viewport = view->GetTargetViewport();
		float halfWidth = viewport.Width / 2.0f;
		float halfHeight = viewport.Height / 2.0f;
		Rect2D<float> screenRect(
			-halfWidth, -halfHeight,
			halfWidth, halfHeight
		);

		// ターゲットが画面内か
		bool isTargetInScreen = Utility::IsPresentInScreen(Vec3(0, 0, 0), view, screenRect);
		SetDrawActive(!isTargetInScreen);
		// ターゲットが画面内
		if (isTargetInScreen) {
			return;
		}

		// カメラ方向を計算
		Vec3 cameraForward = m_camera->GetAt() - m_camera->GetEye();
		cameraForward.normalize();
		// カメラ方向を法線とした平面にターゲット方向へのベクトルを投影する
		Vec3 targetDirectionOnPlane = Utility::Vector3ProjectOnPlane(
			Vec3(0, 0, 0) - m_camera->GetAt(), cameraForward);
		targetDirectionOnPlane.normalize();
		Vec3 cameraUp = m_camera->GetUp().GetNormalized();

		// カメラの上方向（スクリーンの上方向）を基準とした
		// targetDirectionOnPlaneのなす角度を求める
		float angle = Utility::GetTwoVectorAngle360(cameraUp, targetDirectionOnPlane, cameraForward);
		m_rectTrans->SetRotation(XMConvertToRadians(angle));
	}

	void DirectionWithHasBallUI::SetTarget(const shared_ptr<Transform>& target) {
		m_targetObject = target;
	}

	void DirectionWithHasBallUI::ClearTarget() {
		SetTarget(nullptr);
	}
}