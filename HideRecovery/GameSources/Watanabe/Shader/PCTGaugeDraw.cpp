#include "stdafx.h"
#include "PCTGaugeDraw.h"

namespace basecross {
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPCTGaugeDraw)
		IMPLEMENT_DX11_VERTEX_SHADER(VSPCTGaugeDraw, App::GetApp()->GetShadersPath() + L"VSPCTGaugeDraw.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(PSPCTGaugeDraw, App::GetApp()->GetShadersPath() + L"PSPCTGaugeDraw.cso")

		PCTGaugeDraw::PCTGaugeDraw(const shared_ptr<GameObject>& gameObjectPtr)
		:DrawComponent(gameObjectPtr),
		m_rate(0.0f), m_threshold(0.01f), m_isBackground(false),
		m_diffuse(Col4(1)), m_emissive(Col4(0))
	{
		// パイプラインステートをデフォルトの2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	void PCTGaugeDraw::OnCreate() {
	}

	void PCTGaugeDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::CullNone);
		}
		//メッシュリソースの取得
		if (m_meshResource) {
			//シェーダの設定
			DrawMesh(m_meshResource->GetMashData());
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}

	void PCTGaugeDraw::DrawMesh(const MeshPrimData& data) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//NULLのシェーダリソースの準備
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		//サンプラーの準備
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
		//各オブジェクト共通処理
		//シェーダの設定
		//頂点シェーダ
		pD3D11DeviceContext->VSSetShader(VSPCTGaugeDraw::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCTGaugeDraw::GetPtr()->GetInputLayout());
		//ピクセルシェーダ
		pD3D11DeviceContext->PSSetShader(PSPCTGaugeDraw::GetPtr()->GetShader(), nullptr, 0);
		//個別処理
		GaugeDrawConstants sb;
		//コンスタントバッファの作成
		SetConstants(sb);
		//テクスチャ
		auto shTex = m_textureResource.lock();
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBPCTGaugeDraw::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBPCTGaugeDraw::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ストライドとオフセット
		UINT stride = data.m_NumStride;
		UINT offset = 0;
		//描画方法のセット
		pD3D11DeviceContext->IASetPrimitiveTopology(data.m_PrimitiveTopology);
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, data.m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(data.m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		//各レンダリングステートの設定
		//ブレンドステート
		RenderState->SetBlendState(pD3D11DeviceContext, GetBlendState());
		//デプスステンシルステート
		RenderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());
		//テクスチャとサンプラー
		if (shTex) {
			// ゲージ用テクスチャ
			pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
			//サンプラーを設定
			RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
		}
		else {
			//シェーダーリソースもクリア
			pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
			//サンプラーもクリア
			RenderState->SetSamplerAllClear(pD3D11DeviceContext);
		}
		//ラスタライザステートと描画
		RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());
		//描画
		pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
	}

	void PCTGaugeDraw::SetConstants(GaugeDrawConstants& spCb) {
		// 行列の取得
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		// 行列の定義
		Mat4x4 World, ViewMat, ProjMat;
		// ワールド行列の決定
		World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		//転置する
		World.transpose();
		//カメラを得る
		auto CameraPtr = GetGameObject()->OnGetDrawCamera();
		//ビューと射影行列を得る
		ViewMat = CameraPtr->GetViewMatrix();
		//転置する
		ViewMat.transpose();
		//転置する
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.transpose();
		spCb.World = World;
		spCb.View = ViewMat;
		spCb.Projection = ProjMat;

		// エミッシブ
		spCb.Emissive = m_emissive;
		// デフィーズはすべて通す
		spCb.Diffuse = m_diffuse;
		// 割合やしきい値を設定
		// フラグはここで設定できるものだけ設定
		spCb.RatioAndThresholdEtc =
			Vec4(m_rate, m_threshold, m_isBackground ? 1.0f : 0.0f, 0);
	}
}