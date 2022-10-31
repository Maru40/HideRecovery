#include "stdafx.h"
#include "PostProcess.h"

namespace basecross {
	PostProcess::PostProcess()
	{}

	void PostProcess::OnPostDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		UINT width = App::GetApp()->GetGameWidth();
		UINT height = App::GetApp()->GetGameHeight();

		for (auto& postEffect : m_postEffects) {
			postEffect->UpdateConstantBuffer();
			pD3D11DeviceContext->CSSetShader(postEffect->GetShader(), nullptr, 0);
			pD3D11DeviceContext->Dispatch(width, height, 1);
		}
	}
}