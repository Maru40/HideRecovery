#include "stdafx.h"
#include "HasBallUI.h"
#include "../Utility/Utility.h"

namespace basecross {
	HasBallUI::HasBallUI(const shared_ptr<Stage>& stage)
		:HasBallUI(stage, Vec2(0))
	{}

	HasBallUI::HasBallUI(const shared_ptr<Stage>& stage, const Vec2 offset)
		: UIObjectBase(stage, L"HasBallUI"), m_offset(offset)
	{}

	shared_ptr<SimpleSprite> HasBallUI::CreateSprite(const wstring& spriteKey) {
		auto sprite = GetStage()->AddGameObject<SimpleSprite>(
			SimpleSprite::Type::SpriteData, spriteKey
			);
		return sprite;
	}

	void HasBallUI::OnCreate() {
		m_icon = CreateSprite(L"Triangle");

		auto rectTrans = m_icon->GetRectTransform();
		rectTrans->SetScale(0.3f, -0.3f);

		m_icon->SetParent(GetThis<HasBallUI>());

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

	void HasBallUI::OnUpdate() {
		// ターゲットが設定されていない
		if (!m_targetTransform.lock()) {
			// 非表示に
			SetDrawActive(false);
			return;
		}
		const Vec3& targetPosition = m_targetTransform.lock()->GetPosition();

		Vec2 screenPosition(0);

		// ターゲットが画面内か
		bool isTargetInScreen = Utility::IsPresentInScreen(targetPosition, m_view.lock(), m_screenRect, screenPosition);
		// 画面内なら表示
		SetDrawActive(isTargetInScreen);
		// ターゲットが画面外
		if (!isTargetInScreen) {
			return;
		}

		m_selfRectTrans.lock()->SetPosition(screenPosition + m_offset);
	}

	void HasBallUI::SetTarget(const shared_ptr<Transform>& targetTransform) {
		m_targetTransform = targetTransform;
	}

	void HasBallUI::SetTarget(const shared_ptr<GameObject>& targetObject) {
		SetTarget(targetObject->GetComponent<Transform>());
	}

	void HasBallUI::ClearTarget() {
		m_targetTransform = nullptr;
	}
}