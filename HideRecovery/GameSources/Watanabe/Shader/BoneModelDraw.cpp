/*!
@file   BoneModelDraw.cpp
@brief  ボーン付きのモデル描画コンポーネント実体
*/

#include "stdafx.h"
#include "BoneModelDraw.h"

namespace basecross {
	BoneModelDraw::BoneModelDraw(const shared_ptr<GameObject>& owner)
		:SmBaseDraw(owner)
	{}
	BoneModelDraw::~BoneModelDraw() {}

	void BoneModelDraw::SetMeshResource(const shared_ptr<MeshResource>& meshRes) {
		SmBaseDraw::SetMeshResource(meshRes);
		BoneInit();
	}

	void BoneModelDraw::SetMeshResource(const wstring& meshKey) {
		SetMeshResource(App::GetApp()->GetResource<MeshResource>(meshKey));
	}

	void BoneModelDraw::SetMultiMeshResource(const shared_ptr<MultiMeshResource>& meshRes) {
		SmBaseDraw::SetMultiMeshResource(meshRes);
		BoneInit();
	}

	void BoneModelDraw::SetMultiMeshResource(const wstring& meshKey) {
		SetMultiMeshResource(App::GetApp()->GetResource<MultiMeshResource>(meshKey));
	}

	void BoneModelDraw::OnCreate() {
		//ライティングのみだと極端になるので調整
		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	void BoneModelDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			//シェーダの設定
			if (IsOwnShadowActive()) {
				//影付き
				if (GetGameObject()->GetComponent<Shadowmap>(false)) {
					//シャドウマップがあれば自己影防止用のピクセルシェーダ
					DrawModel<VSPNTBoneShadow, PSPNTStaticShadow2>(PtrMeshResource->GetMashData());
				}
				else {
					DrawModel<VSPNTBoneShadow, PSPNTStaticShadow>(PtrMeshResource->GetMashData());
				}
			}
			else {
				DrawModel<VSPNTBone, PSModelDraw>(PtrMeshResource->GetMashData());
			}
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					if (GetOwnShadowActive()) {
						if (GetGameObject()->GetComponent<Shadowmap>(false)) {
							DrawModel<VSPNTBoneShadow, PSPNTStaticShadow2>(vec[i]);
						}
						else {
							DrawModel<VSPNTBoneShadow, PSPNTStaticShadow>(vec[i]);
						}
					}
					else {
						DrawModel<VSPNTBone, PSModelDraw>(vec[i]);
					}
				}
			}
		}

		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}
}