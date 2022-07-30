/*!
@file PNTLineDraw.cpp
@brief PNTLineDrawなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "PNTLineDraw.h"

namespace basecross {

	PNTLineDraw::PNTLineDraw(const std::shared_ptr<GameObject>& objPtr)
		:SmBaseDraw(objPtr)
	{}

	void PNTLineDraw::OnCreate() {
		//SetLightingEnabled(true);
		////マルチライトの設定
		//for (int i = 0; i < GetMaxDirectionalLights(); i++) {
		//	SetLightEnabled(i, true);
		//}

		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	void PNTLineDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}

		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStatic2(PtrMeshResource->GetMashData());
		}
		return;
		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					if (GetOwnShadowActive()) {
						if (GetGameObject()->GetComponent<Shadowmap>(false)) {
							DrawStatic<VSPNTStaticShadow, PSPNTStaticShadow2>(vec[i]);
						}
						else {
							DrawStatic<VSPNTStaticShadow, PSPNTStaticShadow>(vec[i]);
						}
					}
					else {
						DrawStatic<VSPNTStatic, PSPNTStatic>(vec[i]);
					}
				}
			}
		}
		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}


	void PNTLineDraw::DrawStatic2(const MeshPrimData& data) {
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
		pD3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());
		//ピクセルシェーダ
		pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//個別処理
		SimpleConstants SmCb;
		//コンスタントバッファの作成
		SetConstants(SmCb, data);
		//テクスチャ
		auto shTex = GetTextureResource();
		if (shTex) {
			//テクスチャがある
			SmCb.ActiveFlg.x = 1;
		}
		else {
			//描画コンポーネントにはテクスチャがない
			if (shTex = data.m_TextureResource.lock()) {
				//テクスチャがある
				SmCb.ActiveFlg.x = 1;
			}
			else {
				SmCb.ActiveFlg.x = 0;
			}
		}
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &SmCb, 0, 0);
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
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
		//pD3D11DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
		//pD3D11DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
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
		//影とサンプラー
		if (IsOwnShadowActive()) {
			//シャドウマップのレンダラーターゲット
			auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
			ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
			pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
			//シャドウマップサンプラー
			ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
			pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
		}
		//ラスタライザステートと描画
		if (GetRasterizerState() == RasterizerState::DoubleDraw) {
			//透明処理用
			//ラスタライザステート(裏描画)
			pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
			//描画
			pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
			//ラスタライザステート（表描画）
			pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
			//描画
			pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
		}
		else {
			RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());
			//描画
			pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
		}
	}

}