#include "stdafx.h"
#include "RenderTexture.h"

namespace basecross {
	RenderTexture::RenderTexture(const DXGI_FORMAT format) {
		auto device = App::GetApp()->GetDeviceResources()->GetD3DDevice();

		D3D11_TEXTURE2D_DESC textureDesc{};

		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.Format = format;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		textureDesc.Width = App::GetApp()->GetGameWidth();
		textureDesc.Height = App::GetApp()->GetGameHeight();
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;

		ThrowIfFailed(
			device->CreateTexture2D(&textureDesc, nullptr, m_texture.GetAddressOf()),
			L"テクスチャの作成に失敗しました。",
			L"device->CreateTexture2D(&textureDesc, nullptr, &m_texture)",
			L"RenderTexture::RenderTexture()"
		);

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.Format = textureDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;

		ThrowIfFailed(
			device->CreateRenderTargetView(m_texture.Get(), &rtvDesc, m_renderTargetView.GetAddressOf()),
			L"レンダーターゲットビューの作成に失敗しました。",
			L"device->CreateRenderTargetView(m_texture.Get(), &rtvDesc, m_renderTargetView.GetAddressOf())",
			L"RenderTexture::RenderTexture()"
		);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		ThrowIfFailed(
			device->CreateShaderResourceView(m_texture.Get(), &srvDesc, m_shaderResourceView.GetAddressOf()),
			L"シェーダーリソースビューの作成に失敗しました。",
			L"device->CreateShaderResourceView(m_texture.Get(), &srvDesc, m_shaderResourceView.GetAddressOf())",
			L"RenderTexture::RenderTexture()"
		);

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
		uavDesc.Format = textureDesc.Format;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;

		ThrowIfFailed(
			device->CreateUnorderedAccessView(m_texture.Get(), &uavDesc, m_unorderdAccessView.GetAddressOf()),
			L"UAVの作成に失敗しました。",
			L"device->CreateUnorderedAccessView(m_texture.Get(),srvDesc,m_unorderdAccessView.GetAddressOf())",
			L"RenderTexture::RenderTexture(int width, int height, const Col4& color, DXGI_FORMAT format)"
		);
	}

	void RenderTexture::Clear(const Col4& color) {
		auto deviceResource = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = deviceResource->GetD3DDevice();
		auto pD3D11DeviceContext = deviceResource->GetD3DDeviceContext();
		//バッファのクリア
		float Color[4] = { color.x, color.y, color.z, color.w };
		//レンダリングターゲットのクリア
		pD3D11DeviceContext->ClearRenderTargetView(m_renderTargetView.Get(), Color);
	}

	void RenderTexture::Copy(RenderTexture* destTexture, RenderTexture* sourceTexture) {
		auto deviceResource = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = deviceResource->GetD3DDeviceContext();
		pD3D11DeviceContext->CopyResource(destTexture->m_texture.Get(), sourceTexture->m_texture.Get());
	}
}