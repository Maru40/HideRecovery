/*!
@file ToonPNTStaticModelDraw.cpp
@brief ToonPNTStaticModelDrawなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "ToonPNTStaticModelDraw.h"

namespace basecross {

	//コンスタントバッファに渡したいパラメータのみ記述する
	struct ToonPNTStaticModelDraw::Impl : public DrawObjectBase {
		//頂点変更する場合のメッシュ（オリジナル）
		shared_ptr<MeshResource> m_OriginalMeshResource;
		//オリジナルメッシュを使うかどうか
		bool m_UseOriginalMeshResource;
		//テクスチャリソース
		weak_ptr<TextureResource> m_TextureResource;
		//NormalMapテクスチャ
		weak_ptr<TextureResource> m_NormalMapTextureResource;
		vector<weak_ptr<TextureResource>> m_NormalMapTextureResourceVec;
		//エミッシブ色
		Col4 m_Emissive;
		//デフューズ色
		Col4 m_Diffuse;
		//透明度
		float m_Alpha;
		//スペキュラーとパワー色
		Col4 m_SpecularColorAndPower;
		//ライティングするかどうか
		bool m_LightingEnabled;
		//ピクセルライティングをするかどうか
		bool m_PreferPerPixelLighting;
		//法線にバイアスをかけるかどうか
		bool m_BiasedNormals;
		//アンビエント
		Col4 m_AmbientLightColor;
		//ライトのMAX
		static const int MaxDirectionalLights = 3;
		//ライトが有効かどうか
		bool m_LightEnabled[MaxDirectionalLights];
		//ライトの向き
		Vec3 m_LightDirection[MaxDirectionalLights];
		//ライトのデフューズ色
		Col4 m_LightDiffuseColor[MaxDirectionalLights];
		//ライトのスペキュラー色
		Col4 m_LightSpecularColor[MaxDirectionalLights];
		//フォグが有効かどうか
		bool m_FogEnabled;
		//フォグの開始位置
		float m_FogStart;
		//フォグの終了位置
		float m_FogEnd;
		//フォグ色
		Col4 m_FogColor;
		//フォグベクトル
		Vec3 m_FogVector;
		//影を投影するかどうか
		bool m_OwnShadowActive;
		//モデルに入っているDiffuseを使うかどうか
		bool m_ModelDiffusePriority;
		//モデルに入っているEmissiveを使うかどうか
		bool m_ModelEmissivePriority;
		//モデルに入っているテクスチャを使うかどうか
		bool m_ModelTextureEnabled;

		ToonPNTStaticModelDraw::Impl() :
			m_UseOriginalMeshResource(false),
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_Alpha(1.0f),
			m_SpecularColorAndPower(0.0f, 0.0f, 0.0f, 1.0f),
			m_LightingEnabled(false),
			m_PreferPerPixelLighting(false),
			m_BiasedNormals(false),
			m_AmbientLightColor(0, 0, 0, 0),
			m_FogEnabled(false),
			m_FogStart(-25.0f),
			m_FogEnd(-40.0f),
			m_FogColor(0.8f, 0.8f, 0.8f, 1.0f),
			m_FogVector(0.0, 0.0, 1.0f),
			m_OwnShadowActive(false),
			m_ModelDiffusePriority(true),
			m_ModelEmissivePriority(true),
			m_ModelTextureEnabled(true)
		{
			static const XMVECTORF32 defaultSpecular = { 1, 1, 1, 16 };
			static const bsm::Vec3 defaultLightDirection = { 0, -1, 0 };
			m_SpecularColorAndPower = defaultSpecular;
			for (int i = 0; i < MaxDirectionalLights; i++)
			{
				m_LightDirection[i] = defaultLightDirection;
				m_LightDiffuseColor[i] = g_XMZero;
				m_LightSpecularColor[i] = g_XMZero;
			}
		}

	};

	//--------------------------------------------------------------------------------------
	/// トゥーンモデル描画コンポーネント本体
	//--------------------------------------------------------------------------------------

	ToonPNTStaticModelDraw::ToonPNTStaticModelDraw(const std::shared_ptr<GameObject>& objPtr)
		:BcBaseDraw(objPtr)
	{}

	void ToonPNTStaticModelDraw::OnCreate() {
		SetLightingEnabled(true);
		//マルチライトの設定
		for (int i = 0; i < GetMaxDirectionalLights(); i++) {
			SetLightEnabled(i, true);
		}

		SetRampTextureResource(L"Ramp_Rock_TX");
		SetSamplerState(SamplerState::LinearClamp);
	}

	void ToonPNTStaticModelDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}

		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawModel2(PtrMeshResource->GetMashData());
		}

		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					DrawModel2(vec[i]);
				}
			}
		}

		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}

	void ToonPNTStaticModelDraw::DrawModel2(const MeshPrimData& data) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//NULLのシェーダリソースの準備
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		//サンプラーの準備
		ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
		ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
		//各オブジェクト共通処理
		//シェーダの設定
		//頂点シェーダ
		pD3D11DeviceContext->VSSetShader(ToonVS::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(ToonVS::GetPtr()->GetInputLayout());
		//ピクセルシェーダ
		pD3D11DeviceContext->PSSetShader(ToonPS::GetPtr()->GetShader(), nullptr, 0);
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
		//影とサンプラー
		if (GetOwnShadowActive()) {
			//シャドウマップのレンダラーターゲット
			auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
			ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
			pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
			//シャドウマップサンプラー
			ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
			pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
		}
		//ライトを設定
		SetLightingParamaters();
		BasicConstants BcCb;
		//コンスタントバッファの作成
		SetConstants(BcCb, data);
		//メッシュ内のマテリアルの配列
		auto& MatVec = data.m_MaterialExVec;
		size_t MatIndex = 0;
		for (auto& m : MatVec) {
			if (m.m_TextureResource) {
				//テクスチャがある
				BcCb.activeFlg.y = 1;
			}
			else {
				//テクスチャがない
				BcCb.activeFlg.y = 0;
			}
			if (IsModelDiffusePriority()) {
				BcCb.diffuseColor = m.m_Diffuse;
			}
			if (IsModelEmissivePriority()) {
				bsm::Col4 Em4 = m.m_Emissive;
				Em4.w = 0.0f;
				BcCb.emissiveColor = Em4;
			}
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBBasic::GetPtr()->GetBuffer(), 0, nullptr, &BcCb, 0, 0);
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBBasic::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

			//RampTexutreTextrue
			auto rampTex = GetRampTextureResource();
			if (rampTex) {
				//pD3D11DeviceContext->VSSetShaderResources(1, 1, rampTex->GetShaderResourceView().GetAddressOf());
				pD3D11DeviceContext->PSSetShaderResources(1, 1, rampTex->GetShaderResourceView().GetAddressOf());

				//サンプラーを設定
				RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
			}

			if (BcCb.activeFlg.y) {
				//シェーダにテクスチャの設定がされている
				//サンプラーの設定
				RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
				//テクスチャの選択
				if (IsModelTextureEnabled()) {
					//モデルのテクスチャが有効
					pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
				}
				else {
					//モデルのテクスチャが無効
					auto shTex = GetTextureResource();
					if (shTex) {
						//コンポーネントにテクスチャがある
						pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
					}
					else {
						//コンポーネントにテクスチャがない
						BcCb.activeFlg.y = 0;
					}
				}
				if (BcCb.activeFlg.y) {
					//法線マップ
					if (GetNormalMapTextureResourceVec().size() > MatIndex) {
						//複数の法線マップがある
						auto& normalVec = GetNormalMapTextureResourceVec();
						auto normalShptr = normalVec[MatIndex].lock();
						if (GetOwnShadowActive()) {
							//２番目に入れる
							pD3D11DeviceContext->PSSetShaderResources(2, 1, normalShptr->GetShaderResourceView().GetAddressOf());
							//サンプラーを設定
							RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 2);
						}
						else {
							//1番目に入れる
							pD3D11DeviceContext->PSSetShaderResources(1, 1, normalShptr->GetShaderResourceView().GetAddressOf());
							//サンプラーを設定
							RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 1);
						}
					}
					else if (GetNormalMapTextureResource()) {
						//そうではなくて一つだけ法線マップがある
						if (GetOwnShadowActive()) {
							//２番目に入れる
							pD3D11DeviceContext->PSSetShaderResources(2, 1, GetNormalMapTextureResource()->GetShaderResourceView().GetAddressOf());
							//サンプラーを設定
							RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 2);
						}
						else {
							//1番目に入れる
							pD3D11DeviceContext->PSSetShaderResources(1, 1, GetNormalMapTextureResource()->GetShaderResourceView().GetAddressOf());
							//サンプラーを設定
							RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 1);
						}
					}
				}
				else {
					//シェーダーリソースもクリア
					pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
					//サンプラーもクリア
					RenderState->SetSamplerAllClear(pD3D11DeviceContext);
				}
			}
			else {
				if (!rampTex) {
					//シェーダーリソースもクリア
					pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
					//サンプラーもクリア
					RenderState->SetSamplerAllClear(pD3D11DeviceContext);
				}
			}

			//ラスタライザステートと描画
			if (GetRasterizerState() == RasterizerState::DoubleDraw) {
				//透明処理用
				//ラスタライザステート(裏描画)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//描画
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				//ラスタライザステート（表描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else {
				RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());
				//描画
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			MatIndex++;
		}
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void ToonPNTStaticModelDraw::SetRampTextureResource(const std::shared_ptr<TextureResource>& textureResource) {
		m_rampTextureResource = textureResource;
	}

	void ToonPNTStaticModelDraw::SetRampTextureResource(const wstring& key) {
		SetRampTextureResource(App::GetApp()->GetResource<TextureResource>(key));
	}

	std::shared_ptr<TextureResource> ToonPNTStaticModelDraw::GetRampTextureResource() const {
		return m_rampTextureResource;
	}

}