#pragma once
#include "stdafx.h"

namespace basecross {
	class RenderTexture {
		ComPtr<ID3D11Texture2D> m_texture;
		ComPtr<ID3D11RenderTargetView> m_renderTargetView;	//レンダリングターゲットレビュー
		ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
		ComPtr<ID3D11UnorderedAccessView> m_unorderdAccessView;

	public:
		RenderTexture(const DXGI_FORMAT format);
		~RenderTexture() {}

		void Clear(const Col4& col = Col4(0, 0, 0, 1.0f));

		ID3D11Texture2D* GetTexture2D() const { return m_texture.Get(); }
		ID3D11RenderTargetView* GetRenderTargetView() const { return m_renderTargetView.Get(); }
		ID3D11ShaderResourceView* GetShaderResourceView() const { return m_shaderResourceView.Get(); }
		ID3D11UnorderedAccessView* GetUnorderdAccessView() const { return m_unorderdAccessView.Get(); }

		static void Copy(RenderTexture* destTexture, RenderTexture* sourceTexture);
	};
}
