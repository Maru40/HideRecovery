/*!
@file   AdvBaseDraw.h
@brief  カスタム描画クラス
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	struct AdvConstants {
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
		XMUINT4 ActiveFlg;
		// ライイト方向
		Vec4 LightDir;
		// ライト位置
		Vec4 LightPos;
		// Eyeの位置
		Vec4 EyePos;
		// ライトビュー行列
		Mat4x4 LightView;
		// ライト射影行列
		Mat4x4 LightProjection;
		// Bone配列
		Vec4 Bones[3 * 100];
		AdvConstants() {
			memset(this, 0, sizeof(AdvConstants));
			Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};
	class AdvBaseDraw : public DrawComponent {
	private:
		struct Impl;
		unique_ptr<Impl> pImpl;
	protected:
		explicit AdvBaseDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~AdvBaseDraw();

		/// <summary>
		/// コンスタントバッファの設定
		/// </summary>
		/// <param name="cb">設定するコンスタントバッファ構造体</param>
		/// <param name="data">設定するコンスタントバッファ構造体</param>
		void SetConstants(AdvConstants& cb, const MeshPrimData& data);

		/// <summary>
		/// 行列バッファの作成(インスタンス描画用)
		/// </summary>
		//void CreateMatrixBuffer();

		/// <summary>
		/// 行列バッファのマップ(インスタンス描画用)
		/// </summary>
		//void MapMatrixBuffer();

		/// <summary>
		/// スタティックメッシュの描画
		/// </summary>
		/// <typeparam name="T_VShader">使用する頂点シェーダ</typeparam>
		/// <typeparam name="T_PShader">使用するピクセルシェーダ</typeparam>
		/// <param name="data">メッシュのデータ</param>
		template<typename T_VShader, typename T_PShader>
		void DrawStatic(const MeshPrimData& data) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			// NULLのシェーダリソースの準備
			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
			// サンプラーの準備
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			// シェーダの設定
			// 頂点シェーダ
			pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
			// インプットレイアウトの設定
			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());
			// ピクセルシェーダ
			pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);

			// コンスタントバッファの作成
			AdvConstants SmCb;
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
			pD3D11DeviceContext->UpdateSubresource(CBAdvBaseDraw::GetPtr()->GetBuffer(), 0, nullptr, &SmCb, 0, 0);
			// コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBAdvBaseDraw::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
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
				// いずれはまとめてセットしたい
				size_t index = 0;
				for (auto texture : GetAllTextureResource()) {
					// 一枚目は上でセットしているので0番目を1にセットする
					pD3D11DeviceContext->PSSetShaderResources(index, 1, texture.lock()->GetShaderResourceView().GetAddressOf());
					// サンプラーを設定
					RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), index);
					index++;
				}
			}
			else {
				// シェーダーリソースもクリア
				pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
				// サンプラーもクリア
				RenderState->SetSamplerAllClear(pD3D11DeviceContext);
			}
			// 影とサンプラー
			if (IsOwnShadowActive()) {
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

#pragma region DrawStaticInstance
		/// <summary>
		/// スタティックメッシュのインスタンス描画
		/// </summary>
		/// <typeparam name="T_VShader">使用する頂点シェーダ</typeparam>
		/// <typeparam name="T_PShader">使用するピクセルシェーダ</typeparam>
		/// <param name="data">メッシュのデータ</param>
		//template<typename T_VShader, typename T_PShader>
		//void DrawStaticInstance(const MeshPrimData& data) {
		//	auto Dev = App::GetApp()->GetDeviceResources();
		//	auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//	auto RenderState = Dev->GetRenderState();

		//	ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };

		//	ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };

		//	pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);

		//	pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());

		//	pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);

		//	AdvConstants SmCb;

		//	SetConstants(SmCb, data);

		//	auto shTex = GetTextureResource();
		//	if (shTex) {
		//		SmCb.ActiveFlg.x = 1;
		//	}
		//	else {
		//		SmCb.ActiveFlg.x = 0;
		//	}

		//	pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &SmCb, 0, 0);

		//	ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
		//	ID3D11Buffer* pNullConstantBuffer = nullptr;

		//	pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

		//	pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//	UINT stride[2] = { data.m_NumStride, sizeof(Mat4x4) };
		//	UINT offset[2] = { 0, 0 };
		//	ID3D11Buffer* pBuf[2] = { data.m_VertexBuffer.Get(), GetMatrixBuffer().Get() };

		//	pD3D11DeviceContext->IASetPrimitiveTopology(data.m_PrimitiveTopology);

		//	pD3D11DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);

		//	pD3D11DeviceContext->IASetIndexBuffer(data.m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

		//	RenderState->SetBlendState(pD3D11DeviceContext, GetBlendState());

		//	RenderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());

		//	if (shTex) {
		//		pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());

		//		RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
		//	}
		//	else {
		//		pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);

		//		RenderState->SetSamplerAllClear(pD3D11DeviceContext);
		//	}

		//	if (IsOwnShadowActive()) {
		//		auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
		//		ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
		//		pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);

		//		ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
		//		pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
		//	}

		//	if (GetRasterizerState() == RasterizerState::DoubleDraw) {
		//		pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());

		//		pD3D11DeviceContext->DrawIndexedInstanced(data.m_NumIndicis, (UINT)GetMatrixVec().size(), 0, 0, 0);

		//		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//		pD3D11DeviceContext->DrawIndexedInstanced(data.m_NumIndicis, (UINT)GetMatrixVec().size(), 0, 0, 0);
		//	}
		//	else {
		//		RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());

		//		pD3D11DeviceContext->DrawIndexedInstanced(data.m_NumIndicis, (UINT)GetMatrixVec().size(), 0, 0, 0);
		//	}
		//}
#pragma endregion

		/// <summary>
		/// モデルメッシュの描画
		/// </summary>
		/// <typeparam name="T_VShader">使用する頂点シェーダ</typeparam>
		/// <typeparam name="T_PShader">使用するピクセルシェーダ</typeparam>
		/// <param name="data">メッシュのデータ</param>
		template<typename T_VShader, typename T_PShader>
		void DrawModel(const MeshPrimData& data) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			// NULLのシェーダリソースの準備
			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
			// サンプラーの準備
			ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
			ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			// シェーダの設定
			// 頂点シェーダ
			pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
			// インプットレイアウトの設定
			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());
			// ピクセルシェーダ
			pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);
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

			// コンスタントバッファの作成
			AdvConstants SmCb;
			SetConstants(SmCb, data);

			// メッシュ内のマテリアルの配列
			auto& MatVec = data.m_MaterialExVec;
			size_t MatIndex = 0;
			for (auto& m : MatVec) {
				if (m.m_TextureResource) {
					// テクスチャがある
					SmCb.ActiveFlg.x = 1;
				}
				else {
					// テクスチャがない
					SmCb.ActiveFlg.x = 0;
				}
				if (IsModelDiffusePriority()) {
					SmCb.Diffuse = m.m_Diffuse;
				}
				if (IsModelEmissivePriority()) {
					Col4 Em4 = m.m_Emissive;
					Em4.w = 0.0f;
					SmCb.Emissive = Em4;
				}
				// コンスタントバッファの更新
				pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &SmCb, 0, 0);
				// コンスタントバッファの設定
				ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				// 頂点シェーダに渡す
				pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				// ピクセルシェーダに渡す
				pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
				if (SmCb.ActiveFlg.x) {
					// シェーダにテクスチャの設定がされている
					// サンプラーの設定
					RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
					// テクスチャの選択
					if (IsModelTextureEnabled()) {
						// モデルのテクスチャが有効
						pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
						// いずれはまとめてセットしたい
						size_t index = 0;
						for (auto texture : GetAllTextureResource()) {
							// 一枚目は上でセットしているので0番目を1にセットする
							pD3D11DeviceContext->PSSetShaderResources(index + 1, 1, texture.lock()->GetShaderResourceView().GetAddressOf());
							// サンプラーを設定
							RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), index + 1);
							index++;
						}
					}
					else {
						// モデルのテクスチャが無効
						auto shTex = GetTextureResource();
						if (shTex) {
							// コンポーネントにテクスチャがある
							pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
						}
						else {
							// コンポーネントにテクスチャがない
							SmCb.ActiveFlg.x = 0;
						}
					}
				}
				else {
					// シェーダーリソースもクリア
					pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
					// サンプラーもクリア
					RenderState->SetSamplerAllClear(pD3D11DeviceContext);
				}
				// ラスタライザステートと描画
				if (GetRasterizerState() == RasterizerState::DoubleDraw) {
					// 透明処理用
					// ラスタライザステート(裏描画)
					pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
					// 描画
					pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
					// ラスタライザステート（表描画）
					pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
					// 描画
					pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				else {
					RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());
					// 描画
					pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				MatIndex++;
			}
		}
	public:
		/// <summary>
		/// オリジナルメッシュを使うかどうか
		/// </summary>
		/// <returns>使う場合はtrue</returns>
		bool IsOriginalMeshUse() const;
		/// <summary>
		/// オリジナルメッシュを使うかどうかを設定
		/// </summary>
		/// <param name="b">オリジナルメッシュを使うかどうか</param>
		void SetOriginalMeshUse(bool b);
		/// <summary>
		/// オリジナルなメッシュリソースの取得
		/// </summary>
		/// <returns>オリジナルなメッシュリソース</returns>
		shared_ptr<MeshResource> GetOriginalMeshResource() const;
		/// <summary>
		/// オリジナルなメッシュリソースの設定
		/// </summary>
		/// <param name="meshRes">メッシュリソース</param>
		void SetOriginalMeshResource(const shared_ptr<MeshResource>& meshRes);

		/// <summary>
		/// オリジナルメッシュを作成する
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="vertices">頂点の配列</param>
		/// <param name="indices">インデックスの配列</param>
		template <typename T>
		void CreateOriginalMesh(vector<T>& vertices, vector<uint16_t>& indices) {
			try {
				auto meshRes = MeshResource::CreateMeshResource(vertices, indices, true);
				SetOriginalMeshResource(meshRes);
			}
			catch (...) {
				throw;
			}
		}
		/// <summary>
		/// オリジナルメッシュの頂点バッファを変更する
		/// </summary>
		/// <param name="vertices">頂点配列</param>
		template <typename T>
		void UpdateVertices(const vector<T>& vertices) {
			auto meshRes = GetOriginalMeshResource();
			if (!meshRes) {
				throw BaseException(
					L"オリジナルメッシュが作成されていません",
					L"if (!GetOriginalMeshResource())",
					L"StaticBaseDraw::UpdateVertices()"
				);
			}
			meshRes->UpdateVirtexBuffer(vertices);
		}
		/// <summary>
		/// メッシュリソースの取得(オリジナルかどうかは内部で処理される)
		/// </summary>
		/// <returns>メッシュリソース</returns>
		shared_ptr<MeshResource> GetMeshResource() const;
		/// <summary>
		/// オリジナルではないメッシュリソースの設定(仮想関数)
		/// </summary>
		/// <param name="meshRes">メッシュリソース</param>
		virtual void SetMeshResource(const shared_ptr<MeshResource>& meshRes);
		/// <summary>
		/// オリジナルではないメッシュリソースの設定
		/// </summary>
		/// <param name="meshKey">メッシュキー</param>
		void SetMeshResource(const wstring& meshKey);
		/// <summary>
		/// テクスチャリソースの設定
		/// </summary>
		/// <param name="extureRes">テクスチャリソース</param>
		/// <param name="index">インデックス</param>
		void SetTextureResource(const shared_ptr<TextureResource>& textureRes, size_t index);
		/// <summary>
		/// テクスチャリソースの設定
		/// </summary>
		/// <param name="textureKey">登録されているテクスチャキー</param>
		/// <param name="index">インデックス</param>
		void SetTextureResource(const wstring& textureKey, size_t index);
		/// <summary>
		/// テクスチャリソースの取得
		/// </summary>
		/// <param name="index">インデックス</param>
		/// <returns>テクスチャリソース</returns>
		shared_ptr<TextureResource> GetTextureResource(size_t index = 0) const;
		vector<weak_ptr<TextureResource>> GetAllTextureResource() const;
		/// <summary>
		/// マルチメッシュリソースを得る
		/// </summary>
		/// <returns>メッシュリソース</returns>
		shared_ptr<MultiMeshResource> GetMultiMeshResource() const;
		/// <summary>
		/// マルチメッシュリソースを設定する
		/// </summary>
		/// <param name="meshResPtr">メッシュリソース</param>
		virtual void SetMultiMeshResource(const shared_ptr<MultiMeshResource>& meshResPtr);
		/// <summary>
		/// マルチメッシュリソースを設定する
		/// </summary>
		/// <param name="resKey">メッシュリソースのキー</param>
		void SetMultiMeshResource(const wstring& resKey);
		/// <summary>
		/// マルチメッシュの該当のインデックスは描画するかどうかを得る
		/// </summary>
		/// <param name="index">マルチメッシュのインデックス</param>
		/// <returns>描画すればtrue</returns>
		bool GetMultiMeshIsDraw(size_t index) const;
		/// <summary>
		/// マルチメッシュの描画するインデックスを設定する
		/// </summary>
		/// <param name="index">マルチメッシュのインデックス</param>
		/// <param name="b">描画するかどうか</param>
		void SetMultiMeshIsDraw(size_t index, bool b);

		/// <summary>
		/// エミッシブ色の取得
		/// </summary>
		/// <returns>エミッシブ色</returns>
		Col4 GetEmissive() const;
		/// <summary>
		/// エミッシブ色の設定
		/// </summary>
		/// <param name="col">エミッシブ色</param>
		void SetEmissive(const Col4& col);
		/// <summary>
		/// デフューズ色の取得
		/// </summary>
		/// <returns>デフューズ色</returns>
		Col4 GetDiffuse() const;
		/// <summary>
		/// デフューズ色の設定
		/// </summary>
		/// <param name="col">デフューズ色</param>
		void SetDiffuse(const Col4& col);
		/// <summary>
		/// スペキュラー色の取得
		/// </summary>
		/// <returns>スペキュラー色</returns>
		Col4 GetSpecular() const;
		/// <summary>
		/// スペキュラー色の設定
		/// </summary>
		/// <param name="col">スペキュラー色</param>
		void SetSpecular(const Col4& col);
		/// <summary>
		/// 影を描画するかどうか得る
		/// </summary>
		/// <returns>影を描画すればtrue</returns>
		bool GetOwnShadowActive() const;
		/// <summary>
		/// 影を描画するかどうか得る
		/// </summary>
		/// <returns>影を描画すればtrue</returns>
		bool IsOwnShadowActive() const;
		/// <summary>
		/// 影を描画するかどうか設定する
		/// </summary>
		/// <param name="b">影を描画するかどうか</param>
		void SetOwnShadowActive(bool b);

		/// <summary>
		/// デフィーズ色をモデル設定優先かどうか得る
		/// </summary>
		/// <returns>デフィーズ色をモデル設定優先ならtrue</returns>
		bool GetModelDiffusePriority() const;
		/// <summary>
		/// デフィーズ色をモデル設定優先かどうか得る
		/// </summary>
		/// <returns>デフィーズ色をモデル設定優先ならtrue</returns>
		bool IsModelDiffusePriority() const;
		/// <summary>
		/// デフィーズ色をモデル設定優先かどうか設定する
		/// </summary>
		/// <param name="b">デフィーズ色をモデル設定優先かどうか</param>
		void SetModelDiffusePriority(bool b);
		/// <summary>
		/// エミッシブ色をモデル設定優先かどうか得る
		/// </summary>
		/// <returns>エミッシブ色をモデル設定優先ならtrue</returns>
		bool GetModelEmissivePriority() const;
		/// <summary>
		/// エミッシブ色をモデル設定優先かどうか得る
		/// </summary>
		/// <returns>エミッシブ色をモデル設定優先ならtrue</returns>
		bool IsModelEmissivePriority() const;
		/// <summary>
		/// エミッシブ色をモデル設定優先かどうか設定する
		/// </summary>
		/// <param name="b">エミッシブ色をモデル設定優先かどうか</param>
		void SetModelEmissivePriority(bool b);
		/// <summary>
		/// モデルのテクスチャを有効にするかどうか得る
		/// </summary>
		/// <returns>モデルのテクスチャを有効ならtrue</returns>
		bool GetModelTextureEnabled() const;
		/// <summary>
		/// モデルのテクスチャを有効にするかどうか得る
		/// </summary>
		/// <returns>モデルのテクスチャを有効ならtrue</returns>
		bool IsModelTextureEnabled() const;
		/// <summary>
		/// モデルのテクスチャを有効にするかどうか設定する
		/// </summary>
		/// <param name="b">モデルのテクスチャを有効にするかどうか</param>
		void SetModelTextureEnabled(bool b);
		/// <summary>
		/// ボーンの初期化
		/// </summary>
		void BoneInit();

		/// <summary>
		/// 開始行列から終了行列の間のt時間時の行列を返す
		/// </summary>
		/// <param name="m1">開始の行列</param>
		/// <param name="m2">終了の行列</param>
		/// <param name="t">時間（0から1.0f）</param>
		/// <param name="out">結果を受け取る行列</param>
		void InterpolationMatrix(const Mat4x4& m1, const Mat4x4& m2, float t, Mat4x4& out);
		/// <summary>
		/// アニメーションを追加する
		/// </summary>
		/// <param name="name">アニメーション名</param>
		/// <param name="startSample">開始サンプル</param>
		/// <param name="sampleLength">サンプルの長さ</param>
		/// <param name="loop">ループするかどうか</param>
		/// <param name="samplesParSecond">1秒あたりのサンプル数(デフォルト = 30)</param>
		void AddAnimation(const wstring& name, int startSample, int sampleLength, bool loop,
			float samplesParSecond = 30.0f);
		/// <summary>
		/// 現在のアニメーションを変更する
		/// </summary>
		/// <param name="anemationName">アニメーション名（すでに追加されているもの）</param>
		/// <param name="startTime">開始からの秒数</param>
		void ChangeCurrentAnimation(const wstring& anemationName, float startTime = 0.0f);
		/// <summary>
		/// 現在のアニメーション名を得る
		/// </summary>
		/// <returns>アニメーション名（文字列）</returns>
		const wstring& GetCurrentAnimation() const;
		/// <summary>
		/// 現在のアニメーションタイムを得る
		/// </summary>
		/// <returns>アニメーションタイム</returns>
		float GetCurrentAnimationTime() const;
		/// <summary>
		/// 現在のアニメーションが終了しているかどうか
		/// </summary>
		/// <returns>終了していればtrue</returns>
		bool IsTargetAnimeEnd() const;
		/// <summary>
		/// 現在のアニメーションを進める
		/// </summary>
		/// <param name="elapsedTime">経過時間</param>
		/// <returns>アニメーションが終了すればtrue</returns>
		bool UpdateAnimation(float elapsedTime);
		/// <summary>
		/// ローカルボーン行列配列を得る
		/// </summary>
		/// <returns>ローカルボーン行列配列の先頭ポインタ</returns>
		virtual const vector<Mat4x4>* GetVecLocalBonesPtr() const override;
		/// <summary>
		/// ローカルボーン行列配列の参照を得る(書き換え可能)
		/// </summary>
		/// <returns>ローカルボーン行列配列の参照</returns>
		vector<Mat4x4>& GetVecLocalBones();
		/// <summary>
		/// ローカルボーン行列配列を得る（マルチメッシュ版）
		/// </summary>
		/// <param name="index"></param>
		/// <returns>ローカルボーン行列配列の先頭ポインタ</returns>
		virtual const vector<Mat4x4>* GetVecMultiLocalBonesPtr(size_t index) const override;
		/// <summary>
		/// ローカルボーン行列配列の参照を得る(マルチメッシュ版版、書き換え可能)
		/// </summary>
		/// <param name="index"></param>
		/// <returns>ローカルボーン行列配列の参照（範囲外なら例外）</returns>
		vector<Mat4x4>& GetVecMultiLocalBones(size_t index);

		//size_t GetMaxInstance() const;
		//void ResizeMaxInstance(size_t NewSize);
		//void AddMatrix(const Mat4x4& NewMat);
		//void UpdateMultiMatrix(const vector<Mat4x4>& NewMatVec);
		//vector<Mat4x4>& GetMatrixVec() const;
		//void ClearMatrixVec();
		//bool IsAutoClearMatrixVec() const;
		//void SetAutoClearMatrixVec(bool b);
		//ComPtr<ID3D11Buffer>& GetMatrixBuffer() const;

		/// <summary>
		/// メッシュのローカル頂点配列を得る
		/// </summary>
		/// <param name="vertices">受け取る頂点の配列</param>
		void GetStaticMeshLocalPositions(vector<Vec3>& vertices);
		/// <summary>
		/// メッシュのワールド頂点配列を得る
		/// </summary>
		/// <param name="vertices">受け取る頂点の配列</param>
		void GetStaticMeshWorldPositions(vector<Vec3>& vertices);
		/// <summary>
		/// 線分とメッシュのワールド頂点の衝突判定
		/// </summary>
		/// <param name="StartPos">線分の開始点</param>
		/// <param name="EndPos">線分の終了点</param>
		/// <param name="HitPoint">衝突していた場合の衝突点</param>
		/// <param name="RetTri">衝突していた場合の三角形</param>
		/// <param name="RetIndex">衝突していた場合の三角形の位置</param>
		/// <returns>衝突していたらtrue</returns>
		bool HitTestStaticMeshSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, Vec3& HitPoint, TRIANGLE& RetTri, size_t& RetIndex);
		/// <summary>
		/// 球体とメッシュのワールド頂点の衝突判定
		/// </summary>
		/// <param name="StartSp">球体の開始点</param>
		/// <param name="EndSp">球体の終了点</param>
		/// <param name="HitPoint">衝突していた場合の衝突点</param>
		/// <param name="RetTri">衝突していた場合の三角形</param>
		/// <param name="RetIndex">衝突していた場合の三角形の位置</param>
		/// <returns>衝突していたらtrue</returns>
		bool HitTestStaticMeshSphereTriangles(const SPHERE& StartSp, const SPHERE& EndSp, Vec3& HitPoint, TRIANGLE& RetTri, size_t& RetIndex);
		/// <summary>
		/// スキン処理済みのメッシュのローカル頂点配列を得る
		/// </summary>
		/// <param name="vertices">受け取る頂点の配列</param>
		void GetSkinedMeshLocalPositions(vector<Vec3>& vertices);
		/// <summary>
		/// スキン処理済みのメッシュのワールド頂点配列を得る
		/// </summary>
		/// <param name="vertices">受け取る頂点の配列</param>
		void GetSkinedMeshWorldPositions(vector<Vec3>& vertices);
		/// <summary>
		/// 線分とスキンメッシュのワールド頂点の衝突判定
		/// </summary>
		/// <param name="StartPos">線分の開始点</param>
		/// <param name="EndPos">線分の終了点</param>
		/// <param name="HitPoint">衝突していた場合の衝突点</param>
		/// <param name="RetTri">衝突していた場合の三角形</param>
		/// <param name="RetIndex">衝突していた場合の三角形の位置</param>
		/// <returns>衝突していたらtrue</returns>
		bool HitTestSkinedMeshSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, Vec3& HitPoint, TRIANGLE& RetTri, size_t& RetIndex);
		/// <summary>
		/// 球体とスキンメッシュのワールド頂点の衝突判定
		/// </summary>
		/// <param name="StartSp">球体の開始点</param>
		/// <param name="EndSp">球体の終了点</param>
		/// <param name="HitPoint">衝突していた場合の衝突点</param>
		/// <param name="RetTri">衝突していた場合の三角形</param>
		/// <param name="RetIndex">衝突していた場合の三角形の位置</param>
		/// <returns>衝突していたらtrue</returns>
		bool HitTestSkinedMeshSphereTriangles(const SPHERE& StartSp, const SPHERE& EndSp, Vec3& HitPoint, TRIANGLE& RetTri, size_t& RetIndex);
	};
}
