#include "stdafx.h"
#include "BarrierShader.h"

namespace basecross {
	BarrierShader::BarrierShader(const shared_ptr<GameObject>& owner)
		:AdvBaseDraw(owner)
	{}
	BarrierShader::~BarrierShader() {}

	void BarrierShader::OnCreate() {
		// ライティングのみだと極端になるので調整
		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	// 影を使用しないためディゾブルの同期はしない
	void BarrierShader::OnUpdate() {}

	void BarrierShader::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			// 影は描画しない
			DrawStatic<VSModelDraw, PSBarrierDraw>(PtrMeshResource->GetMashData());
		}

		//マルチメッシュには未対応

		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}
}