/*!
@file   PCTGaugeSprite.cpp
@brief  ゲージ描画コンポーネントの実体
*/

#include "stdafx.h"
#include "PCTGaugeSprite.h"

namespace basecross {
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPCTGaugeSprite)
		IMPLEMENT_DX11_VERTEX_SHADER(VSPCTGaugeSprite, App::GetApp()->GetShadersPath() + L"VSPCTGaugeSprite.cso")
		IMPLEMENT_DX11_PIXEL_SHADER(PSPCTGaugeSprite, App::GetApp()->GetShadersPath() + L"PSPCTGaugeSprite.cso")

		PCTGaugeSprite::PCTGaugeSprite(const shared_ptr<GameObject>& gameObjectPtr)
		:MySpriteBaseDraw(gameObjectPtr),
		m_rate(0.0f), m_threshold(0.01f), m_gaugeGradientTexKey(L""),
		m_isBackground(false), m_maximum(1.0f)
	{
		// パイプラインステートをデフォルトの2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	PCTGaugeSprite::PCTGaugeSprite(const shared_ptr<GameObject>& gameObjectPtr,
		vector<VertexPositionColorTexture>& vertices, vector<uint16_t>& indices)
		:MySpriteBaseDraw(gameObjectPtr),
		m_rate(0.0f), m_threshold(0.01f), m_gaugeGradientTexKey(L""),
		m_isBackground(false), m_maximum(1.0f)
	{
		// パイプラインステートをデフォルトの2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
		try {
			CreateMesh(vertices, indices);
		}
		catch (...) {
			throw;
		}
	}

	void PCTGaugeSprite::OnCreate() {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			//作成されてなかったら作成する
			float HelfSize = 0.5f;
			//頂点配列
			vector<VertexPositionColorTexture> vertices = {
				{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
				{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(1.0f, 0.0f)) },
				{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 1.0f)) },
				{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f,1.0f,1.0f,1.0f), Vec2(1.0f, 1.0f)) },
			};
			//インデックス配列
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//メッシュの作成（変更できる）
			CreateMesh(vertices, indices);
		}
		GetGameObject()->SetSpriteDraw(true);
	}

	void PCTGaugeSprite::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::CullNone);
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			//シェーダの設定
			DrawSprite(PtrMeshResource->GetMashData());
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}

	void PCTGaugeSprite::DrawSprite(const MeshPrimData& data) {
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
		pD3D11DeviceContext->VSSetShader(VSPCTGaugeSprite::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCTGaugeSprite::GetPtr()->GetInputLayout());
		//ピクセルシェーダ
		pD3D11DeviceContext->PSSetShader(PSPCTGaugeSprite::GetPtr()->GetShader(), nullptr, 0);
		//個別処理
		GaugeSpriteConstants sb;
		//コンスタントバッファの作成
		SetConstants(sb);
		//テクスチャ
		auto shTex = GetTextureResource();
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBPCTGaugeSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBPCTGaugeSprite::GetPtr()->GetBuffer();
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
			// グラデーションテクスチャがセットされている場合は読み込みを行う
			if (m_gaugeGradientTexKey != L"") {
				// グラデーションテクスチャ
				pD3D11DeviceContext->PSSetShaderResources(1, 1,
					App::GetApp()->GetResource<TextureResource>(m_gaugeGradientTexKey)->GetShaderResourceView().GetAddressOf());
				// フラグをオンに
				sb.RatioAndThresholdEtc.z = 1.0f;
			}
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

	void PCTGaugeSprite::SetConstants(GaugeSpriteConstants& spCb) {
		// 行列の取得
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		// 行列の定義
		Mat4x4 World, Proj;
		World = PtrTrans->Get2DWorldMatrix();
		// 射影行列の決定
		GetGameObject()->OnGet2DDrawProjMatrix(Proj);
		// 行列の合成
		World *= Proj;
		// エミッシブ
		spCb.Emissive = GetEmissive();
		// デフィーズはすべて通す
		spCb.Diffuse = GetDiffuse();
		// 行列の設定
		spCb.World = World;
		// 割合やしきい値を設定
		// フラグはここで設定できるものだけ設定
		spCb.RatioAndThresholdEtc =
			Vec4(m_rate, m_threshold, 0, m_isBackground ? 1.0f : 0.0f);
		spCb.Maximum = Vec4(m_maximum, 0, 0, 0);
	}
}