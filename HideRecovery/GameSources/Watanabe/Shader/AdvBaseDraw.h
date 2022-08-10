#pragma once
#include "stdafx.h"

namespace basecross {
	struct AdvSimpleConstants {
		// ワールド行列
		Mat4x4 World;
		// ビュー行列
		Mat4x4 View;
		// 射影行列
		Mat4x4 Projection;
		// エミッシブ色
		Col4 Emissive;
		// デフューズ色
		Col4 Diffuse;
		// スペキュラー
		Col4 Specular;
		// テクスチャ=xがアクティブかどうか
		Vec4 ActiveFlg;
		// Eyeの位置
		Vec4 EyePos;
		// Bone配列
		Vec4 Bones[3 * 72];
		AdvSimpleConstants() {
			memset(this, 0, sizeof(AdvSimpleConstants));
			Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};

	class AdvSmBaseDraw : public DrawComponent {
	protected:
		explicit AdvSmBaseDraw(const shared_ptr<GameObject>& GameObjectPtr);

		virtual ~AdvSmBaseDraw();

		void SetConstants(AdvSimpleConstants& SmCb, const MeshPrimData& data);

		template <typename T_VShader, typename T_PShader>
		void DrawStatic(const MeshPrimData& data) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			// NULLのシェーダリソースの準備
			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
			// サンプラーの準備
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			// 各オブジェクト共通処理
			// シェーダの設定
			// 頂点シェーダ
			pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
			// インプットレイアウトの設定
			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());
			// ピクセルシェーダ
			pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);
			// 個別処理
			AdvSimpleConstants SmCb;
			// コンスタントバッファの作成
			SetConstants(SmCb, data);
			// テクスチャ
			auto shTex = GetTextureResource();
			if (shTex) {
				// テクスチャがある
				SmCb.ActiveFlg.x = 1;
			}
			else {
				// 描画コンポーネントにはテクスチャがない
				if (shTex = data.m_TextureResource.lock()) {
					// テクスチャがある
					SmCb.ActiveFlg.x = 1;
				}
				else {
					SmCb.ActiveFlg.x = 0;
				}
			}
			// コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBAdvSimple::GetPtr()->GetBuffer(), 0, nullptr, &SmCb, 0, 0);
			// コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBAdvSimple::GetPtr()->GetBuffer();
			// 頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			// ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

			// ストライドとオフセット
			UINT stride = data.m_NumStride;
			UINT offset = 0;

			// 描画方法のセット
			pD3D11DeviceContext->IASetPrimitiveTopology(data.m_PrimitiveTopology);
			// 頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, data.m_VertexBuffer.GetAddressOf(), &stride, &offset);
			// インデックスバッファのセット
			pD3D11DeviceContext->IASetIndexBuffer(data.m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
			// 各レンダリングステートの設定
			// ブレンドステート
			RenderState->SetBlendState(pD3D11DeviceContext, GetBlendState());
			// デプスステンシルステート
			RenderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());
			// テクスチャとサンプラー
			if (shTex) {
				pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
				// サンプラーを設定
				RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
			}
			else {
				// シェーダーリソースもクリア
				pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
				// サンプラーもクリア
				RenderState->SetSamplerAllClear(pD3D11DeviceContext);
			}
			// 影とサンプラー
			if (GetOwnShadowActive()) {
				// シャドウマップのレンダラーターゲット
				auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
				ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
				pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
				// シャドウマップサンプラー
				ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
				pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
			}
			// ラスタライザステートと描画
			if (GetRasterizerState() == RasterizerState::DoubleDraw) {
				// 透明処理用
				// ラスタライザステート(裏描画)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				// 描画
				pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
				// ラスタライザステート（表描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				// 描画
				pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
			}
			else {
				RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());
				// 描画
				pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
			}
		}

	public:
		/**
		 * @brief オリジナルメッシュを使うかどうか
		 *
		 * @return 使う場合はtrue
		 */
		bool IsOriginalMeshUse() const;
		/**
		 * @brief オリジナルメッシュを使うかどうかを設定
		 *
		 * @param b オリジナルメッシュを使うかどうか
		 */
		void SetOriginalMeshUse(bool b);
		/**
		 * @brief オリジナルなメッシュリソースの取得
		 *
		 * @return オリジナルなメッシュリソース
		 */
		shared_ptr<MeshResource> GetOriginalMeshResource() const;
		/**
		 * @brief オリジナルなメッシュリソースの設定
		 *
		 * @param MeshRes メッシュリソース
		 */
		void SetOriginalMeshResource(const shared_ptr<MeshResource>& MeshRes);
		/**
		 * @brief オリジナルメッシュを作成する
		 *
		 * @param Vertices 頂点の配列
		 * @param indices インデックスの配列
		 */
		template <typename T>
		void CreateOriginalMesh(vector<T>& Vertices, vector<uint16_t>& indices) {
			try {
				auto MeshRes = MeshResource::CreateMeshResource(Vertices, indices, true);
				SetOriginalMeshResource(MeshRes);
			}
			catch (...) {
				throw;
			}
		}
		/**
		 * @brief オリジナルメッシュの頂点バッファを変更する
		 *
		 * @param Vertices 頂点配列
		 */
		template <typename T>
		void UpdateVertices(const vector<T>& Vertices) {
			auto MeshRes = GetOriginalMeshResource();
			if (!MeshRes) {
				throw BaseException(
					L"オリジナルメッシュが作成されていません",
					L"if (!GetOriginalMeshResource())",
					L"StaticBaseDraw::UpdateVertices()"
				);
			}
			MeshRes->UpdateVirtexBuffer(Vertices);
		}

		shared_ptr<MeshResource> GetMeshResource() const;

		virtual void SetMeshResource(const shared_ptr<MeshResource>& MeshRes);

		void SetMeshResource(const wstring& MeshKey);

		void SetTextureResource(const shared_ptr<TextureResource>& TextureRes);

		void SetTextureResource(const wstring& TextureKey);

		shared_ptr<TextureResource> GetTextureResource() const;

		Col4 GetEmissive() const;

		void SetEmissive(const Col4& col);

		Col4 GetDiffuse() const;

		void SetDiffuse(const Col4& col);

		Col4 GetSpecular() const;

		void SetSpecular(const Col4& col);

		bool GetOwnShadowActive() const;

		void SetOwnShadowActive(bool b);

	private:
		struct Impl;
		unique_ptr<Impl> pImpl;
	};
}
