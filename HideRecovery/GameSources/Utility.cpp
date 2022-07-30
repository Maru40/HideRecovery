#include "stdafx.h"
#include "Utility.h"

namespace basecross {
	Vec3 Utility::ConvertWorldToScreen(const shared_ptr<ViewBase>& view, const Vec3& position)
	{
		const auto camera = view->GetTargetCamera();
		const auto viewport = view->GetTargetViewport();
		float w = viewport.Width * 0.5f;  // 画面の幅の半分
		float h = viewport.Height * 0.5f; // 画面の高さの半分

		auto mView = camera->GetViewMatrix(); // ビュー行列
		auto mProj = camera->GetProjMatrix(); // プロジェクション(射影)行列
		Mat4x4 mScreen( // スクリーン行列（BaseCross用）
			{ w, 0, 0, 0 },
			{ 0, h, 0, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 }
		);

		Mat4x4 mVPS = mView * mProj * mScreen;

		return Vec3(XMVector3TransformCoord(position, mVPS));
	}
}