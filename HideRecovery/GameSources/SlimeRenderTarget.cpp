/*!
@file SlimeRenderTarget.cpp
@brief SlimeRenderTarget�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "SlimeRenderTarget.h"

namespace basecross {

	struct SlimeRenderTarget::Impl {
		//�r���[�֘A
		const float m_slimeDrawDimension;                           //���@
		ComPtr<ID3D11ShaderResourceView>	m_ShaderResourceView;	//�V�F�[�_���\�[�X�r���[
		ComPtr<ID3D11RenderTargetView> m_D3D11RenderTargetView;	    //�����_�����O�^�[�Q�b�g���r���[
		ComPtr<ID3D11Texture2D>		m_DepthStencil;		            //�[�x�X�e���V���o�b�t�@
		ComPtr<ID3D11Texture2D>		m_test;		
		ComPtr<ID3D11DepthStencilView>	m_DepthStencilView;	        //�[�x�X�e���V���r���[
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

			////�����_�����O�^�[�Q�b�g�r���[�̍쐬
			//ComPtr<ID3D11Texture2D> pBackBuffer;
			////�܂��o�b�N�o�b�t�@�̃|�C���^�𓾂�
			//ThrowIfFailed(
			//	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer),
			//	L"�X���b�v�`�F�[������o�b�N�o�b�t�@�̎擾�Ɏ��s���܂����B",
			//	L"pSwapChain->GetBuffer(0, __uuidof(ID3D1s1Texture2D), (LPVOID*)&pBackBuffer)",
			//	L"DefaultRenderTarget::DefaultRenderTarget()"
			//);
			////�o�b�N�o�b�t�@���烌���_�����O�^�[�Q�b�g�̃r���[���쐬����
			//ThrowIfFailed(
			//	pD3D11Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pImpl->m_D3D11RenderTargetView),
			//	L"DX11�o�b�N�o�b�t�@����̃����_�����O�^�[�Q�b�g�r���[���쐬�Ɏ��s���܂����B",
			//	L"pD3D11Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_D3D11RenderTargetView)",
			//	L"DefaultRenderTarget::DefaultRenderTarget()"
			//);



			//�����_�����O��̃e�N�X�`���̍쐬
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
				L"DX11�e�N�X�`���̍쐬���s�̍쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateTexture2D(&descTexture, nullptr, &pImpl->m_test)",
				L"TADrawRenderTarget::TADrawRenderTarget()"
			);

			ThrowIfFailed(
				pD3D11Device->CreateRenderTargetView(pImpl->m_test.Get(), 0, &pImpl->m_D3D11RenderTargetView),
				L"DX11�e�N�X�`������̃����_�����O�^�[�Q�b�g�r���[���쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateRenderTargetView(pImpl->m_test.Get(), 0, &pImpl->m_D3D11RenderTargetView)",
				L"TADrawRenderTarget::TADrawRenderTarget()"
			);

			//�[�x�e�N�X�`���̍쐬
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
				L"DX11�[�x�e�N�X�`���̍쐬���s�̍쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateTexture2D(&descDepth, nullptr, &m_DepthStencil)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
			);

			//�[�x�X�e���V���r���[�̍쐬
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;

			ThrowIfFailed(
				pD3D11Device->CreateDepthStencilView(pImpl->m_DepthStencil.Get(), &descDSV, &pImpl->m_DepthStencilView),
				L"DX11�[�x�X�e���V���r���[�̍쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateDepthStencilView(m_DepthStencil.Get(), &descDSV, &m_DepthStencilView)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
			);

			//�V�F�[�_�[���\�[�X�r���[�쐬
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			shaderResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;

			ThrowIfFailed(
				pD3D11Device->CreateShaderResourceView(pImpl->m_test.Get(), &shaderResourceViewDesc, &pImpl->m_ShaderResourceView),
				L"�V�F�[�_�[���\�[�X�r���[�̍쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateShaderResourceView(m_DepthStencil.Get(), &shaderResourceViewDesc, &m_ShaderResourceView)",
				L"TAAreaMapRenderTarget::TAAreaMapRenderTarget()"
			);

			//ComPtr<IDXGISurface2> dxgiBackBuffer;
			//ThrowIfFailed(
			//	pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)),
			//	L"2d�f�o�C�X�R���e�L�X�g�쐬�Ɏ��s���܂����B",
			//	L"m_d2dDevice->CreateDeviceContext()",
			//	L"DeviceResources::Impl::CreateDeviceResources()"
			//);


			//ThrowIfFailed(
			//	pD2D11DeviceContext->CreateBitmapFromDxgiSurface(
			//		dxgiBackBuffer.Get(),
			//		nullptr,	//�f�t�H���g�ݒ�
			//		&pImpl->m_d2dTargetBitmap
			//	),
			//	L"2d�r�b�g�}�b�v�쐬�Ɏ��s���܂����B",
			//	L"pD2D11DeviceContext->CreateBitmapFromDxgiSurface()",
			//	L"DefaultRenderTarget::DefaultRenderTarget()"
			//);

			//pD2D11DeviceContext->SetTarget(pImpl->m_d2dTargetBitmap.Get());
			////�O���[�X�P�[�� �e�L�X�g�̃A���`�G�C���A�V���O
			//pD2D11DeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);


			//�f�t�H���g�r���[�|�[�g�̃Z�b�g
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

	//�A�N�Z�T-------------------------------------------------------------------

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

	//����------------------------------------------------------------------------
	
	//�X�N���[���S�̂��w��̐F�ŃN���A����
	void SlimeRenderTarget::ClearViews(const bsm::Col4& col) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�o�b�t�@�̃N���A
		float Color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		D3D11_VIEWPORT ViewPort;
		//�r���[�|�[�g�̃Z�b�g�A�b�v
		ZeroMemory(&ViewPort, sizeof(ViewPort));
		ViewPort.Width = (float)App::GetApp()->GetGameWidth();
		ViewPort.Height = (float)App::GetApp()->GetGameHeight();
		ViewPort.MinDepth = 0.0f;
		ViewPort.MaxDepth = 1.0f;
		ViewPort.TopLeftX = 0;
		ViewPort.TopLeftY = 0;
		pD3D11DeviceContext->RSSetViewports(1, &ViewPort);
		//�����_�����O�^�[�Q�b�g�̃N���A
		pD3D11DeviceContext->ClearRenderTargetView(pImpl->m_D3D11RenderTargetView.Get(), Color);
		//�ʏ�̐[�x�o�b�t�@�ƃX�e���V���o�b�t�@�̃N���A
		pD3D11DeviceContext->ClearDepthStencilView(pImpl->m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	//�����_�����O�^�[�Q�b�g���J�n����
	void SlimeRenderTarget::StartRenderTarget() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		ID3D11RenderTargetView* pV = pImpl->m_D3D11RenderTargetView.Get();
		//�����_�����O�^�[�Q�b�g�ƃX�e���V����ݒ�
		pD3D11DeviceContext->OMSetRenderTargets(1, &pV, pImpl->m_DepthStencilView.Get());
		//�r���[�|�[�g�̐ݒ�
		auto ViewPort = GetViewport();
		pD3D11DeviceContext->RSSetViewports(1, &ViewPort);

		D3D11_RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = (LONG)ViewPort.Width;
		rect.bottom = (LONG)ViewPort.Height;
		pD3D11DeviceContext->RSSetScissorRects(1, &rect);

		//�V�F�[�_�[���\�[�X�r���[�̃N���A
		ID3D11ShaderResourceView* pNull[1] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
		pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
		//�V�F�[�_�[�͎w�肵�Ȃ�
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		//�u�����h�͎w�肵�Ȃ�
		pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	//�����_�����O�^�[�Q�b�g���I������
	void SlimeRenderTarget::EndRenderTarget() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�V�F�[�_�[���\�[�X�r���[�̃N���A
		ID3D11ShaderResourceView* pNull[1] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
		pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
		//�V�F�[�_�[�͎w�肵�Ȃ�
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		//�u�����h�͎w�肵�Ȃ�
		pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

}