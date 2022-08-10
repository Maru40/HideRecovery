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
	protected:
		explicit AdvBaseDraw(const shared_ptr<GameObject>& GameObjectPtr);

		virtual ~AdvBaseDraw();

		void SetConstants(AdvConstants& SmCb, const MeshPrimData& data);

		// void CreateMatrixBuffer();
		// void MapMatrixBuffer();

		template<typename T_VShader, typename T_PShader>
		void DrawStatic(const MeshPrimData& data) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();

			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };

			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };

			pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);

			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());

			pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);

			AdvConstants SmCb;

			SetConstants(SmCb, data);

			auto shTex = GetTextureResource();
			if (shTex) {
				SmCb.ActiveFlg.x = 1;
			}
			else {
				if (shTex = data.m_TextureResource.lock()) {
					SmCb.ActiveFlg.x = 1;
				}
				else {
					SmCb.ActiveFlg.x = 0;
				}
			}

			pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &SmCb, 0, 0);

			ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;

			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

			UINT stride = data.m_NumStride;
			UINT offset = 0;

			pD3D11DeviceContext->IASetPrimitiveTopology(data.m_PrimitiveTopology);

			pD3D11DeviceContext->IASetVertexBuffers(0, 1, data.m_VertexBuffer.GetAddressOf(), &stride, &offset);

			pD3D11DeviceContext->IASetIndexBuffer(data.m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

			RenderState->SetBlendState(pD3D11DeviceContext, GetBlendState());

			RenderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());

			if (shTex) {
				pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());

				RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
			}
			else {
				pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);

				RenderState->SetSamplerAllClear(pD3D11DeviceContext);
			}

			if (IsOwnShadowActive()) {
				auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
				ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
				pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);

				ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
				pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
			}

			if (GetRasterizerState() == RasterizerState::DoubleDraw) {
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());

				pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);

				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

				pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
			}
			else {
				RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());

				pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
			}
		}

#pragma region DrawStaticInstance
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

		template<typename T_VShader, typename T_PShader>
		void DrawModel(const MeshPrimData& data) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();

			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };

			ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
			ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };

			pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);

			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());

			pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);

			UINT stride = data.m_NumStride;
			UINT offset = 0;

			pD3D11DeviceContext->IASetPrimitiveTopology(data.m_PrimitiveTopology);

			pD3D11DeviceContext->IASetVertexBuffers(0, 1, data.m_VertexBuffer.GetAddressOf(), &stride, &offset);

			pD3D11DeviceContext->IASetIndexBuffer(data.m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

			RenderState->SetBlendState(pD3D11DeviceContext, GetBlendState());

			RenderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());

			if (GetOwnShadowActive()) {
				auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
				ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
				pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);

				ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
				pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
			}

			AdvConstants SmCb;

			SetConstants(SmCb, data);

			auto& MatVec = data.m_MaterialExVec;
			size_t MatIndex = 0;
			for (auto& m : MatVec) {
				if (m.m_TextureResource) {
					SmCb.ActiveFlg.x = 1;
				}
				else {
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

				pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &SmCb, 0, 0);

				ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
				ID3D11Buffer* pNullConstantBuffer = nullptr;

				pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

				pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
				if (SmCb.ActiveFlg.x) {
					RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);

					if (IsModelTextureEnabled()) {
						pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
					}
					else {
						auto shTex = GetTextureResource();
						if (shTex) {
							pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
						}
						else {
							SmCb.ActiveFlg.x = 0;
						}
					}
				}
				else {
					pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);

					RenderState->SetSamplerAllClear(pD3D11DeviceContext);
				}

				if (GetRasterizerState() == RasterizerState::DoubleDraw) {
					pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());

					pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);

					pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

					pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				else {
					RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());

					pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				MatIndex++;
			}
		}
	public:

		bool IsOriginalMeshUse() const;

		void SetOriginalMeshUse(bool b);

		shared_ptr<MeshResource> GetOriginalMeshResource() const;

		void SetOriginalMeshResource(const shared_ptr<MeshResource>& MeshRes);

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

		shared_ptr<MultiMeshResource> GetMultiMeshResource() const;

		virtual void SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr);

		void SetMultiMeshResource(const wstring& ResKey);

		bool GetMultiMeshIsDraw(size_t index) const;

		void SetMultiMeshIsDraw(size_t index, bool b);

		Col4 GetEmissive() const;

		void SetEmissive(const Col4& col);

		Col4 GetDiffuse() const;

		void SetDiffuse(const Col4& col);

		Col4 GetSpecular() const;

		void SetSpecular(const Col4& col);

		bool GetOwnShadowActive() const;

		bool IsOwnShadowActive() const;

		void SetOwnShadowActive(bool b);

		bool GetModelDiffusePriority() const;

		bool IsModelDiffusePriority() const;

		void SetModelDiffusePriority(bool b);

		bool GetModelEmissivePriority() const;

		bool IsModelEmissivePriority() const;

		void SetModelEmissivePriority(bool b);

		bool GetModelTextureEnabled() const;

		bool IsModelTextureEnabled() const;

		void SetModelTextureEnabled(bool b);

		void BoneInit();

		void InterpolationMatrix(const Mat4x4& m1, const Mat4x4& m2, float t, Mat4x4& out);

		void AddAnimation(const wstring& Name, int StartSample, int SampleLength, bool Loop,
			float SamplesParSecond = 30.0f);

		void ChangeCurrentAnimation(const wstring& AnemationName, float StartTime = 0.0f);

		const wstring& GetCurrentAnimation() const;

		float GetCurrentAnimationTime() const;

		bool IsTargetAnimeEnd() const;

		bool UpdateAnimation(float ElapsedTime);

		virtual const vector< Mat4x4 >* GetVecLocalBonesPtr() const override;

		vector< Mat4x4 >& GetVecLocalBones();

		virtual const vector< Mat4x4 >* GetVecMultiLocalBonesPtr(size_t index) const override;

		vector< Mat4x4 >& GetVecMultiLocalBones(size_t index);

		//size_t GetMaxInstance() const;
		//void ResizeMaxInstance(size_t NewSize);
		//void AddMatrix(const Mat4x4& NewMat);
		//void UpdateMultiMatrix(const vector<Mat4x4>& NewMatVec);
		//vector<Mat4x4>& GetMatrixVec() const;
		//void ClearMatrixVec();
		//bool IsAutoClearMatrixVec() const;
		//void SetAutoClearMatrixVec(bool b);
		//ComPtr<ID3D11Buffer>& GetMatrixBuffer() const;

		void GetStaticMeshLocalPositions(vector<Vec3>& vertices);

		void GetStaticMeshWorldPositions(vector<Vec3>& vertices);

		bool HitTestStaticMeshSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, Vec3& HitPoint, TRIANGLE& RetTri, size_t& RetIndex);

		bool HitTestStaticMeshSphereTriangles(const SPHERE& StartSp, const SPHERE& EndSp, Vec3& HitPoint, TRIANGLE& RetTri, size_t& RetIndex);

		void GetSkinedMeshLocalPositions(vector<Vec3>& vertices);

		void GetSkinedMeshWorldPositions(vector<Vec3>& vertices);

		bool HitTestSkinedMeshSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, Vec3& HitPoint, TRIANGLE& RetTri, size_t& RetIndex);

		bool HitTestSkinedMeshSphereTriangles(const SPHERE& StartSp, const SPHERE& EndSp, Vec3& HitPoint, TRIANGLE& RetTri, size_t& RetIndex);
	private:

		struct Impl;
		unique_ptr<Impl> pImpl;
	};
}
