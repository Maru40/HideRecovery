#pragma once
#include "stdafx.h"
#include "../Shader/RenderTexture.h"

namespace basecross {
	DECLARE_DX11_VERTEX_SHADER(VS_Main, VertexPosition)
	DECLARE_DX11_PIXEL_SHADER(PS_Main)

	class AdvRenderer :public RenderTarget {
		RenderTexture m_mainRenderTarget;

		ComPtr<ID3D11Texture2D>		m_DepthStencil;		//深度ステンシルバッファ
		ComPtr<ID3D11DepthStencilView>	m_DepthStencilView;	//深度ステンシルビュー

		ComPtr<ID3D11ShaderResourceView> m_depthSrv;
		ComPtr<ID3D11ShaderResourceView> m_stencilSrv;

		ComPtr<ID3D11RasterizerState> m_rasterizerState;
		ComPtr<ID3D11BlendState> m_blendState;
		ComPtr<ID3D11SamplerState> m_samplerState;

	public:
		AdvRenderer();
		~AdvRenderer();

		void ClearViews(const bsm::Col4& col = bsm::Col4(0.0f, 0.0f, 0.0f, 1.0f)) override;

		void StartRenderTarget()override;

		void RenderMain();

		void RenderPostProcessing();

		void PassRenderMain();

		void EndRenderTarget()override;
	};
}
