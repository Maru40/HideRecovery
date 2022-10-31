#include "stdafx.h"
#include "OutlinePostEffect.h"

namespace basecross {
	IMPLEMENT_DX11_CONSTANT_BUFFER(OutlineCB)
		IMPLEMENT_DX11_COMPUTE_SHADER(CS_Outline, App::GetApp()->GetShadersPath() + L"CS_Outline.cso")

		OutlinePostEffect::OutlinePostEffect() {}

	void OutlinePostEffect::UpdateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		auto* constantBuffer = OutlineCB::GetPtr()->GetBuffer();

		OutlineConstructBuffer buffer{};
		buffer.fogCoefficient.x = 30 / (30 - 10);
		buffer.fogCoefficient.y = -1.0f / (30 - 10);
		buffer.windowWidth = App::GetApp()->GetGameWidth();
		buffer.windowHeight = App::GetApp()->GetGameHeight();
		buffer.fogColor = Col4(0);

		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(constantBuffer, 0, nullptr, &buffer, 0, 0);
		pD3D11DeviceContext->CSSetConstantBuffers(1, 1, &constantBuffer);
	}

	ID3D11ComputeShader* OutlinePostEffect::GetShader() const {
		return CS_Outline::GetPtr()->GetShader();
	}
}