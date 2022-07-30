/*!
@file SlimeRenderTarget.cpp
@brief SlimeRenderTargetのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "SlimeRenderTarget.h"

namespace basecross {

	struct SlimeRenderTarget::Impl {
		//ビュー関連
		const float m_slimeDrawDimension;                           //寸法
		ComPtr<ID3D11ShaderResourceView>	m_ShaderResourceView;	//シェーダリソースビュー
		ComPtr<ID3D11RenderTargetView> m_D3D11RenderTargetView;	    //レンダリングターゲットレビュー
		ComPtr<ID3D11Texture2D>		m_DepthStencil;		            //深度ステンシルバッファ
		ComPtr<ID3D11Texture2D>		m_test;		
		ComPtr<ID3D11DepthStencilView>	m_DepthStencilView;	        //深度ステンシルビュー
		ComPtr<ID2D1Bitmap1>		m_d2dTargetBitmap;

		Impl(float slimeDrawDimension)
			:m_slimeDrawDimension(slimeDrawDimension)
		{}
		~Impl() {}
	};

	SlimeRenderTarget::SlimeRenderTarget(float slimeDrawDimension)
		:pImpl(new Impl(slimeDrawDimension))
	{
		try {

			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11Device = Dev->GetD3DDevice();
			auto pSwapChain = Dev->GetSwapChain();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto pD2D11DeviceContext = Dev->GetD2DDeviceContext();

			////レンダリングターゲットビューの作成
			//ComPtr<ID3D11Texture2D> pBackBuffer;
			////まずバックバッファのポインタを得る
			//ThrowIfFailed(
			//	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer),
			//	L"スワップチェーンからバックバッファの取得に失敗しました。",
			//	L"pSwapChain->GetBuffer(0, __uuidof(ID3D1s1Texture2D), (LPVOID*)&pBackBuffer)",
			//	L"DefaultRenderTarget::DefaultRenderTarget()"
			//);
			////バックバッファからレンダリングターゲットのビューを作成する
			//ThrowIfFailed(
			//	pD3D11Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pImpl->m_D3D11RenderTargetView),
			//	L"DX11バックバッファからのレンダリングターゲットビューを作成に失敗しました。",
			//	L"pD3D11Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_D3D11RenderTargetView)",
			//	L"DefaultRenderTarget::DefaultRenderTarget()"
			//);



			//レンダリング先のテクスチャの作成
			D3D11_TEXTURE2D_DESC descTexture;
			ZeroMemory(&descTexture, sizeof(descTexture));
			//descTexture.Width = static_cast<UINT>(pImpl->m_slimeDrawDimension);
			//descTexture.Height = static_cast<UINT>(pImpl->m_slimeDrawDimension);
			descTexture.Width = App::GetApp()->GetGameWidth();
			descTexture.Height = App::GetApp()->GetGameHeight();
			descTexture.MipLevels = 1;
			descTexture.ArraySize = 1;
			descTexture.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			descTexture.SampleDesc.Count = 1;
			descTexture.SampleDesc.Quality = 0;
			descTexture.Usage = D3D11_USAGE_DEFAULT;
			descTexture.BindFlags = D3D10_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			descTexture.CPUAccessFlags = 0;
			descTexture.MiscFlags = 0;

			ThrowIfFailed(
				pD3D11Device->CreateTexture2D(&descTexture, nullptr, &pImpl->m_test),
				L"DX11テクスチャの作成失敗の作成に失敗しました。",
				L"pD3D11Device->CreateTexture2D(&descTexture, nullptr, &pImpl->m_test)",
				L"TADrawRenderTarget::TADrawRenderTarget()"
			);

			ThrowIfFailed(
				pD3D11Device->CreateRenderTargetView(pImpl->m_test.Get(), 0, &pImpl->m_D3D11RenderTargetView),
				L"DX11テクスチャからのレンダリングターゲットビューを作成に失敗しました。",
				L"pD3D11Device->CreateRenderTargetView(pImpl->m_test.Get(), 0, &pImpl->m_D3D11RenderTargetView)",
				L"TADrawRenderTarget::TADrawRenderTarget()"
			);

			//深度テクスチャの作成
			D3D11_TEXTURE2D_DESC descDepth;
			ZeroMemory(&descDepth, sizeof(descDepth));
			//descDepth.Width = static_cast<UINT>(pImpl->m_slimeDrawDimension);
			//descDepth.Height = static_cast<UINT>(pImpl->m_slimeDrawDimension);
			descDepth.Width = App::GetApp()->GetGameWidth();
			descDepth.Height = App::GetApp()->GetGameHeight();
			descDepth.MipLevels = 1;
			descDepth.ArraySize = 1;
			descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDepth.SampleDesc.Count = 1;
			descDepth.SampleDesc.Quality = 0;
			descDepth.Usage = D3D11_USAGE_DEFAULT;
			descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			descDepth.CPUAccessFlags = 0;
			descDepth.MiscFlags = 0;

			ThrowIfFailed(
				pD3D11Device->CreateTexture2D(&descDepth, nullptr, &pImpl->m_DepthStencil),
				L"DX11深度テクスチャの作成失敗の作成に失敗しました。",
				L"pD3D11Device->CreateTexture2D(&descDepth, nullptr, &m_DepthStencil)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
			);

			//深度ステンシルビューの作成
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;

			ThrowIfFailed(
				pD3D11Device->CreateDepthStencilView(pImpl->m_DepthStencil.Get(), &descDSV, &pImpl->m_DepthStencilView),
				L"DX11深度ステンシルビューの作成に失敗しました。",
				L"pD3D11Device->CreateDepthStencilView(m_DepthStencil.Get(), &descDSV, &m_DepthStencilView)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
			);

			//シェーダーリソースビュー作成
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;

			ThrowIfFailed(
				pD3D11Device->CreateShaderResourceView(pImpl->m_test.Get(), &shaderResourceViewDesc, &pImpl->m_ShaderResourceView),
				L"シェーダーリソースビューの作成に失敗しました。",
				L"pD3D11Device->CreateShaderResourceView(m_DepthStencil.Get(), &shaderResourceViewDesc, &m_ShaderResourceView)",
				L"TAAreaMapRenderTarget::TAAreaMapRenderTarget()"
			);

			//ComPtr<IDXGISurface2> dxgiBackBuffer;
			//ThrowIfFailed(
			//	pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)),
			//	L"2dデバイスコンテキスト作成に失敗しました。",
			//	L"m_d2dDevice->CreateDeviceContext()",
			//	L"DeviceResources::Impl::CreateDeviceResources()"
			//);


			//ThrowIfFailed(
			//	pD2D11DeviceContext->CreateBitmapFromDxgiSurface(
			//		dxgiBackBuffer.Get(),
			//		nullptr,	//デフォルト設定
			//		&pImpl->m_d2dTargetBitmap
			//	),
			//	L"2dビットマップ作成に失敗しました。",
			//	L"pD2D11DeviceContext->CreateBitmapFromDxgiSurface()",
			//	L"DefaultRenderTarget::DefaultRenderTarget()"
			//);

			//pD2D11DeviceContext->SetTarget(pImpl->m_d2dTargetBitmap.Get());
			////グレースケール テキストのアンチエイリアシング
			//pD2D11DeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);


			//デフォルトビューポートのセット
			D3D11_VIEWPORT ViewPort;
			ZeroMemory(&ViewPort, sizeof(ViewPort));
			//ViewPort.Width = pImpl->m_slimeDrawDimension;
			//ViewPort.Height = pImpl->m_slimeDrawDimension;
			ViewPort.Width = (float)App::GetApp()->GetGameWidth();
			ViewPort.Height = (float)App::GetApp()->GetGameHeight();
			ViewPort.MinDepth = 0.0f;
			ViewPort.MaxDepth = 1.0f;
			ViewPort.TopLeftX = 0;
			ViewPort.TopLeftY = 0;
			SetViewport(ViewPort);

		}
		catch(...){
			throw;
		}
	}

	SlimeRenderTarget::~SlimeRenderTarget() {}

	//アクセサ-------------------------------------------------------------------

	ID3D11RenderTargetView* SlimeRenderTarget::GetRenderTargetView() const { 
		return pImpl->m_D3D11RenderTargetView.Get(); 
	}
	ID3D11Texture2D* SlimeRenderTarget::GetDepthStencil() const { 
		return pImpl->m_DepthStencil.Get(); 
	}
	ID3D11DepthStencilView* SlimeRenderTarget::GetDepthStencilView() const { 
		return pImpl->m_DepthStencilView.Get(); 
	}
	ID3D11ShaderResourceView* SlimeRenderTarget::GetShaderResourceView() const { 
		return pImpl->m_ShaderResourceView.Get();
	}

	//----------------------------------------------------------------------------

	//操作------------------------------------------------------------------------
	
	//スクリーン全体を指定の色でクリアする
	void SlimeRenderTarget::ClearViews(const bsm::Col4& col) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//バッファのクリア
		float Color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		D3D11_VIEWPORT ViewPort;
		//ビューポートのセットアップ
		ZeroMemory(&ViewPort, sizeof(ViewPort));
		ViewPort.Width = (float)App::GetApp()->GetGameWidth();
		ViewPort.Height = (float)App::GetApp()->GetGameHeight();
		ViewPort.MinDepth = 0.0f;
		ViewPort.MaxDepth = 1.0f;
		ViewPort.TopLeftX = 0;
		ViewPort.TopLeftY = 0;
		pD3D11DeviceContext->RSSetViewports(1, &ViewPort);
		//レンダリングターゲットのクリア
		pD3D11DeviceContext->ClearRenderTargetView(pImpl->m_D3D11RenderTargetView.Get(), Color);
		//通常の深度バッファとステンシルバッファのクリア
		pD3D11DeviceContext->ClearDepthStencilView(pImpl->m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	//レンダリングターゲットを開始する
	void SlimeRenderTarget::StartRenderTarget() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		ID3D11RenderTargetView* pV = pImpl->m_D3D11RenderTargetView.Get();
		//レンダリングターゲットとステンシルを設定
		pD3D11DeviceContext->OMSetRenderTargets(1, &pV, pImpl->m_DepthStencilView.Get());
		//ビューポートの設定
		auto ViewPort = GetViewport();
		pD3D11DeviceContext->RSSetViewports(1, &ViewPort);

		D3D11_RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = (LONG)ViewPort.Width;
		rect.bottom = (LONG)ViewPort.Height;
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
	}

	//レンダリングターゲットを終了する
	void SlimeRenderTarget::EndRenderTarget() {
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