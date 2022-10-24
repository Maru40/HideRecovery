#include "stdafx.h"
#include "PCTStaticModelDraw.h"

namespace basecross
{
	IMPLEMENT_DX11_PIXEL_SHADER(PS_PCTStaticModel, App::GetApp()->GetShadersPath() + L"PSPCTStaticModel.cso")

	PCTStaticModelDraw::PCTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PCTStaticModelDraw::~PCTStaticModelDraw() {}

	void PCTStaticModelDraw::OnCreate() {
	}

	void PCTStaticModelDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawModel<VSPNTStatic, PS_PCTStaticModel>(PtrMeshResource->GetMashData());
		}
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					DrawModel<VSPNTStatic, PSPCTStatic>(vec[i]);
				}
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();

	}
}