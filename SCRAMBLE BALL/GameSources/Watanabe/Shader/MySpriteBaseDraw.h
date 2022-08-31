#pragma once
#include "stdafx.h"

namespace basecross {
	class MySpriteBaseDraw :public DrawComponent {
	private:
		// メッシュ
		shared_ptr<MeshResource> m_spriteMesh;
		// テクスチャリソース
		weak_ptr<TextureResource> m_textureResource;
		// エミッシブ色
		Col4 m_emissive;
		// デフューズ色
		Col4 m_diffuse;
	protected:
		/**
		 * @brief コンストラクタ
		 *
		 * @param gameObjectPtr ゲームオブジェクトのポインタ
		 */
		explicit MySpriteBaseDraw(const shared_ptr<GameObject>& gameObjectPtr);
		/**
		 * @brief デストラクタ
		 */
		virtual ~MySpriteBaseDraw();
	public:
		/**
		 * @brief 頂点変更できるスプライトメッシュを作成する（すでにある場合は差し替える）
		 *
		 * @param Vertices 頂点の配列
		 * @param indices インデックスの配列
		 */
		template<typename T>
		void CreateMesh(vector<T>& Vertices, vector<uint16_t>& indices) {
			try {
				//メッシュの作成（変更できる）
				auto SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
				SetMeshResource(SpriteMesh);
			}
			catch (...) {
				throw;
			}
		}

		/**
		 * @brief 頂点バッファの更新
		 *
		 * @param Vertices 頂点配列
		 */
		template<typename T>
		void UpdateVertices(const vector<T>& Vertices) {
			auto SpriteMesh = GetMeshResource();
			if (!SpriteMesh) {
				throw BaseException(
					L"メッシュが作成されていません",
					L"if (!GetMeshResource())",
					L"SpriteBaseDraw::UpdateVertices()"
				);
			}
			if (Vertices.size() > SpriteMesh->GetNumVertices()) {
				throw BaseException(
					L"更新する頂点が大きすぎます",
					L"if (Vertices.size() > SpriteMesh->GetNumVertices())",
					L"SpriteBaseDraw::UpdateVertices()"
				);
			}
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

			//頂点の変更
			//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
			D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
			D3D11_MAPPED_SUBRESOURCE mappedBuffer;
			//頂点のマップ
			if (FAILED(pD3D11DeviceContext->Map(SpriteMesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer))) {
				// Map失敗
				throw BaseException(
					L"頂点のMapに失敗しました。",
					L"if(FAILED(pID3D11DeviceContext->Map()))",
					L"SpriteBaseDraw::UpdateVertices()"
				);
			}
			//頂点の変更
			T* vertices
				= (T*)mappedBuffer.pData;
			for (size_t i = 0; i < SpriteMesh->GetNumVertices(); i++) {
				vertices[i] = Vertices[i];
			}
			//アンマップ
			pD3D11DeviceContext->Unmap(SpriteMesh->GetVertexBuffer().Get(), 0);
		}

		/**
		 * @brief メッシュリソースの取得
		 *
		 * @return メッシュリソース
		 */
		shared_ptr<MeshResource> GetMeshResource() const;

		/**
		 * @brief メッシュリソースの設定
		 *
		 * @param MeshRes メッシュリソース
		 */
		void SetMeshResource(const shared_ptr<MeshResource>& MeshRes);

		/**
		 * @brief テクスチャリソースの設定
		 *
		 * @param TextureRes テクスチャリソース
		 */
		void SetTextureResource(const shared_ptr<TextureResource>& TextureRes);

		/**
		 * @brief テクスチャリソースの設定
		 *
		 * @param TextureKey 登録されているテクスチャキー
		 */
		void SetTextureResource(const wstring& TextureKey);

		/**
		 * @brief テクスチャリソースの取得
		 *
		 * @return テクスチャリソース
		 */
		shared_ptr<TextureResource> GetTextureResource() const;

		/**
		 * @brief エミッシブ色の取得
		 *
		 * @return エミッシブ色
		 */
		Col4 GetEmissive() const;

		/**
		 * @brief エミッシブ色の設定
		 *
		 * @param col エミッシブ色
		 */
		void SetEmissive(const Col4& col);

		/**
		 * @brief デフューズ色の取得
		 *
		 * @return デフューズ色
		 */
		Col4 GetDiffuse() const;

		/**
		 * @brief デフューズ色の設定
		 *
		 * @param col デフューズ色
		 */
		void SetDiffuse(const Col4& col);
	};
}