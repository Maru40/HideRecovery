/*!
@file   StaticModelDraw.cpp
@brief  静的モデルの描画コンポーネント実体
*/

#include "stdafx.h"
#include "StaticModelDraw.h"

namespace basecross {
	StaticModelDraw::StaticModelDraw(const shared_ptr<GameObject>& owner)
		:AdvBaseDraw(owner)
	{}
	StaticModelDraw::~StaticModelDraw() {}

	void StaticModelDraw::OnCreate() {
		// ライティングのみだと極端になるので調整
		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	void StaticModelDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			//if (GetOwnShadowActive()) {
			//	DrawStatic<VSPNTStaticShadow, PSPNTStaticShadow>(PtrMeshResource->GetMashData());
			//}
			//else {
			DrawStatic<VSModelDraw, PSModelDraw>(PtrMeshResource->GetMashData());
			//}
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					//if (GetOwnShadowActive()) {
					//	if (GetGameObject()->GetComponent<Shadowmap>(false)) {
					//		DrawStatic<VSPNTStaticShadow, PSPNTStaticShadow2>(vec[i]);
					//	}
					//	else {
					//		DrawStatic<VSPNTStaticShadow, PSPNTStaticShadow>(vec[i]);
					//	}
					//}
					//else {
					DrawStatic<VSModelDraw, PSModelDraw>(vec[i]);
					//}
				}
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}
}