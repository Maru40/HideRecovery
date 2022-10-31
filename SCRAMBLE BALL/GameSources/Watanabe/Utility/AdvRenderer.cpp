#include "stdafx.h"
#include "AdvRenderer.h"

namespace basecross {
	IMPLEMENT_DX11_VERTEX_SHADER(VS_Main, App::GetApp()->GetShadersPath() + L"VS_Main.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(PS_Main, App::GetApp()->GetShadersPath() + L"PS_Main.cso")

		AdvRenderer::AdvRenderer() :
		RenderTarget(),
		m_mainRenderTarget(DXGI_FORMAT_R16G16B16A16_FLOAT)
	{
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pSwapChain = Dev->GetSwapChain();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto pD2D11DeviceContext = Dev->GetD2DDeviceContext();

		//深度テクスチャの作成
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = App::GetApp()->GetGameWidth();
		descDepth.Height = App::GetApp()->GetGameHeight();
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		ThrowIfFailed(
			pD3D11Device->CreateTexture2D(&descDepth, nullptr, m_DepthStencil.GetAddressOf()),
			L"DX11深度テクスチャの作成に失敗しました。",
			L"pD3D11Device->CreateTexture2D(&descDepth, nullptr, m_DepthStencil.GetAddressOf())",
			L"AdvRenderer::AdvRenderer()"
		);

		//深度ステンシルビューの作成
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Flags = 0;
		descDSV.Texture2D.MipSlice = 0;

		ThrowIfFailed(
			pD3D11Device->CreateDepthStencilView(m_DepthStencil.Get(), &descDSV, m_DepthStencilView.GetAddressOf()),
			L"DX11深度ステンシルビューの作成に失敗しました。",
			L"pD3D11Device->CreateDepthStencilView(m_DepthStencil.Get(), &descDSV, m_DepthStencilView.GetAddressOf())",
			L"AdvRenderer::AdvRenderer()"
		);

		descDSV.Flags = D3D11_DSV_READ_ONLY_DEPTH | D3D11_DSV_READ_ONLY_STENCIL;

		ThrowIfFailed(
			pD3D11Device->CreateDepthStencilView(m_DepthStencil.Get(), &descDSV, m_DepthStencilViewReadOnly.GetAddressOf()),
			L"DX11深度ステンシルビューの作成に失敗しました。",
			L"pD3D11Device->CreateDepthStencilView(m_DepthStencil.Get(), &descDSV, m_DepthStencilViewReadOnly.GetAddressOf())",
			L"LightPrePassRenderTarget::LightPrePassRenderTarget()"
		);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		ThrowIfFailed(
			pD3D11Device->CreateShaderResourceView(m_DepthStencil.Get(), &srvDesc, m_depthSrv.GetAddressOf()),
			L"DX11深度シェーダーリソースビューの作成に失敗しました。",
			L"pD3D11Device->CreateShaderResourceView(m_DepthStencil.Get(), &srvDesc, m_depthSrv.GetAddressOf())",
			L"AdvRenderer::AdvRenderer()"
		);

		srvDesc.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;

		ThrowIfFailed(
			pD3D11Device->CreateShaderResourceView(m_DepthStencil.Get(), &srvDesc, m_stencilSrv.GetAddressOf()),
			L"DX11深度シェーダーリソースビューの作成に失敗しました。",
			L"pD3D11Device->CreateShaderResourceView(m_DepthStencil.Get(), &srvDesc, m_stencilSrv.GetAddressOf())",
			L"AdvRenderer::AdvRenderer()"
		);

		//デフォルトビューポートのセット
		D3D11_VIEWPORT ViewPort;
		ZeroMemory(&ViewPort, sizeof(ViewPort));
		ViewPort.Width = (float)App::GetApp()->GetGameWidth();
		ViewPort.Height = (float)App::GetApp()->GetGameHeight();
		ViewPort.MinDepth = 0.0f;
		ViewPort.MaxDepth = 1.0f;
		ViewPort.TopLeftX = 0;
		ViewPort.TopLeftY = 0;
		SetViewport(ViewPort);

		D3D11_RASTERIZER_DESC rasterizerDesc{};
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.DepthClipEnable = true;

		pD3D11Device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);

		D3D11_BLEND_DESC blendDescDR{};
		blendDescDR.AlphaToCoverageEnable = false;
		blendDescDR.IndependentBlendEnable = false;
		blendDescDR.RenderTarget[0].BlendEnable = false;
		blendDescDR.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		pD3D11Device->CreateBlendState(&blendDescDR, &m_blendState);
	}

	AdvRenderer::~AdvRenderer() {}

	void AdvRenderer::ClearViews(const Col4& color) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//バッファのクリア
		float Color[4] = { color.x, color.y, color.z, color.w };

		//ビューポートのセットアップ
		pD3D11DeviceContext->RSSetViewports(1, &GetViewport());
		//レンダリングターゲットのクリア
		pD3D11DeviceContext->ClearRenderTargetView(m_mainRenderTarget.GetRenderTargetView(), Color);
		//通常の深度バッファとステンシルバッファのクリア
		pD3D11DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		pD3D11DeviceContext->ClearDepthStencilView(m_DepthStencilViewReadOnly.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		App::GetApp()->GetDeviceResources()->GetDefaultRenderTarget()->ClearViews(color);
	}

	void AdvRenderer::StartRenderTarget() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();

		ClearViews();

		ID3D11RenderTargetView* pV = m_mainRenderTarget.GetRenderTargetView();
		//レンダリングターゲットとステンシルを設定
		pD3D11DeviceContext->OMSetRenderTargets(1, &pV, m_DepthStencilView.Get());

		//ビューポートの設定
		const auto& viewPort = GetViewport();
		pD3D11DeviceContext->RSSetViewports(1, &viewPort);

		D3D11_RECT rect{};
		rect.left = 0;
		rect.top = 0;
		rect.right = (LONG)viewPort.Width;
		rect.bottom = (LONG)viewPort.Height;
		pD3D11DeviceContext->RSSetScissorRects(1, &rect);

		//シェーダーリソースビューのクリア
		ID3D11ShaderResourceView* pNull[1] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
		pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
		//シェーダーは指定しない
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		//ブレンドは指定しない
		pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
		//pD3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
	}

	void AdvRenderer::RenderMain() {
		auto pD3D11DeviceContext = App::GetApp()->GetDeviceResources()->GetD3DDeviceContext();
		auto RenderStatePtr = App::GetApp()->GetDeviceResources()->GetRenderState();

		//ID3D11RenderTargetView* pV = m_mainRenderTarget.GetRenderTargetView();
		//pD3D11DeviceContext->OMSetRenderTargets(1, &pV, m_DepthStencilView.Get());

		//pD3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
	}

	void AdvRenderer::RenderPostProcessing() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		ID3D11ShaderResourceView* srvs[] = { m_depthSrv.Get() };
		pD3D11DeviceContext->CSSetShaderResources(0, _countof(srvs), srvs);

		ID3D11UnorderedAccessView* uavs[] = { m_mainRenderTarget.GetUnorderdAccessView() };
		pD3D11DeviceContext->CSSetUnorderedAccessViews(0, _countof(uavs), uavs, nullptr);
	}

	void AdvRenderer::PassRenderMain() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		ID3D11UnorderedAccessView* pNull[1] = { nullptr };
		pD3D11DeviceContext->CSSetUnorderedAccessViews(0, _countof(pNull), pNull, nullptr);

		ID3D11RenderTargetView* rtvs[] = { App::GetApp()->GetDeviceResources()->GetDefaultRenderTarget()->GetRenderTargetView() };

		pD3D11DeviceContext->OMSetRenderTargets(_countof(rtvs), rtvs, m_DepthStencilViewReadOnly.Get());

		ID3D11ShaderResourceView* srvs[] = { m_mainRenderTarget.GetShaderResourceView(),m_depthSrv.Get() };

		pD3D11DeviceContext->PSSetShaderResources(0, _countof(srvs), srvs);

		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		pD3D11DeviceContext->VSSetShader(VS_Main::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->IASetInputLayout(VS_Main::GetPtr()->GetInputLayout());
		pD3D11DeviceContext->PSSetShader(PS_Main::GetPtr()->GetShader(), nullptr, 0);

		float blendFactor[] = { 0,0,0,0 };
		pD3D11DeviceContext->OMSetBlendState(m_blendState.Get(), blendFactor, 0xffffffff);
		pD3D11DeviceContext->RSSetState(m_rasterizerState.Get());

		pD3D11DeviceContext->Draw(3, 0);
	}

	void AdvRenderer::EndRenderTarget() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//シェーダーリソースビューのクリア
		ID3D11ShaderResourceView* pNull[1] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
		pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
		//シェーダーは指定しない
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		//ブレンドは指定しない
		pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}
}