/*!
@file   DirectionWithHasBallUI.cpp
@brief  画面外のオブジェクト位置を指すUIの実体
*/

#include "stdafx.h"
#include "DirectionWithHasBallUI.h"
#include "../Utility/Utility.h"

namespace basecross {
	DirectionWithHasBallUI::DirectionWithHasBallUI(const shared_ptr<Stage>& stage)
		:UIObjectBase(stage, L"DirectionWithHasBallUI")
	{}

	DirectionWithHasBallUI::DirectionWithHasBallUI(const shared_ptr<Stage>& stage, const wstring& line)
		: UIObjectBase(stage, L"DirectionWithHasBallUI")
	{}

	void DirectionWithHasBallUI::OnCreate() {
		m_camera = GetStage()->GetView()->GetTargetCamera();
		m_cursor = GetStage()->AddGameObject<SimpleSprite>(
			SimpleSprite::Type::Texture, L"Ball_IconTX"
			);

		auto rectTrans = m_cursor->GetRectTransform();
		rectTrans->SetPosition(Vec2(0, 400));
		rectTrans->SetScale(0.3f, -0.3f);

		m_cursor->SetParent(GetThis<DirectionWithHasBallUI>());

		m_view = GetStage()->GetView();

		// 画面内の範囲を定義
		// Viewportを0基準からー～＋範囲に変換
		const Viewport& viewport = m_view.lock()->GetTargetViewport();
		float halfWidth = viewport.Width / 2.0f;
		float halfHeight = viewport.Height / 2.0f;
		Rect2D<float> screenRect(
			-halfWidth, -halfHeight,
			halfWidth, halfHeight
		);
		m_screenRect = screenRect;

		m_selfRectTrans = GetRectTransform();
	}

	void DirectionWithHasBallUI::OnUpdate() {
		// ターゲットが設定されていない
		if (!m_targetTransform.lock()) {
			// 非表示に
			SetDrawActive(false);
			return;
		}
		const Vec3& targetPosition = m_targetTransform.lock()->GetPosition();

		// ターゲットが画面内か
		bool isTargetInScreen = Utility::IsPresentInScreen(targetPosition, m_view.lock(), m_screenRect);
		// 画面外なら表示
		SetDrawActive(!isTargetInScreen);
		// ターゲットが画面内
		if (isTargetInScreen) {
			// 以下の処理はやらない
			return;
		}

		auto camera = m_camera.lock();

		// カメラ方向を計算
		Vec3 cameraForward = camera->GetAt() - camera->GetEye();
		cameraForward.normalize();
		// カメラ方向を法線とした平面にターゲット方向へのベクトルを投影する
		Vec3 targetDirectionOnPlane = Utility::Vector3ProjectOnPlane(
			targetPosition - camera->GetAt(), cameraForward);
		targetDirectionOnPlane.normalize();
		Vec3 cameraUp = camera->GetUp().GetNormalized();

		// カメラの上方向（スクリーンの上方向）を基準とした
		// targetDirectionOnPlaneのなす角度を求める
		float angle = Utility::GetTwoVectorAngle360(cameraUp, targetDirectionOnPlane, cameraForward);
		m_selfRectTrans.lock()->SetRotation(XMConvertToRadians(angle));
	}

	void DirectionWithHasBallUI::SetTarget(const shared_ptr<Transform>& targetTransform) {
		m_targetTransform = targetTransform;
	}

	void DirectionWithHasBallUI::SetTarget(const shared_ptr<GameObject>& targetObject) {
		SetTarget(targetObject->GetComponent<Transform>());
	}

	void DirectionWithHasBallUI::ClearTarget() {
		m_targetTransform.reset();
	}

	bool DirectionWithHasBallUI::IsTargetSet() {
		// リンクが切れていたらターゲットがセットされていないのでfalse
		return !m_targetTransform.expired();
	}
}