#include "stdafx.h"
#include "AdvSmBaseDraw.h"
#include "DrawComponents.h"

namespace basecross {
	struct AdvSmBaseDraw::Impl :public DrawObjectBase {
		//頂点変更する場合のメッシュ（オリジナル）
		shared_ptr<MeshResource> m_OriginalMeshResource;
		//オリジナルメッシュを使うかどうか
		bool m_UseOriginalMeshResource;
		///テクスチャリソース
		weak_ptr<TextureResource> m_TextureResource;
		///エミッシブ色
		Col4 m_Emissive;
		/// デフューズ色
		Col4 m_Diffuse;
		/// スペキュラー色
		Col4 m_Specular;
		///影を投影するかどうか
		bool m_OwnShadowActive;
		//モデルに入っているDiffuseを使うかどうか
		bool m_ModelDiffusePriority;
		//モデルに入っているEmissiveを使うかどうか
		bool m_ModelEmissivePriority;
		//モデルに入っているテクスチャを使うかどうか
		bool m_ModelTextureEnabled;
		///Instance描画用
		///Instance最大値
		size_t m_MaxInstance;
		/// 行列用の頂点バッファ
		ComPtr<ID3D11Buffer> m_MatrixBuffer;
		///行列の配列
		vector<Mat4x4> m_MatrixVec;
		//行列を自動クリアするかどうか
		bool m_AutoClearMatrixVec;
		AdvSmBaseDraw::Impl() :
			m_UseOriginalMeshResource(false),
			m_Emissive(0, 0, 0, 0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_Specular(0, 0, 0, 0),
			m_OwnShadowActive(false),
			m_ModelDiffusePriority(false),
			m_ModelEmissivePriority(false),
			m_ModelTextureEnabled(true),
			m_MaxInstance(2000),
			m_AutoClearMatrixVec(false)
		{}
	};

	AdvSmBaseDraw::AdvSmBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	AdvSmBaseDraw::~AdvSmBaseDraw() {}

	void AdvSmBaseDraw::SetConstants(AdvSimpleConstants& SmCb, const MeshPrimData& data) {
		//行列の定義
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//行列の定義
		Mat4x4 World, ViewMat, ProjMat;
		//ワールド行列の決定
		if (data.m_UseMeshToTransformMatrix) {
			World = data.m_MeshToTransformMatrix * GetMeshToTransformMatrix();
			World *= PtrTrans->GetWorldMatrix();
		}
		else {
			World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		}
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
		SmCb.World = World;
		SmCb.View = ViewMat;
		SmCb.Projection = ProjMat;
		//エミッシブ
		SmCb.Emissive = GetEmissive();
		//デフィーズ
		SmCb.Diffuse = GetDiffuse();
		//スペキュラー
		SmCb.Specular = GetSpecular();

		SmCb.EyePos = CameraPtr->GetEye();
		SmCb.EyePos.w = 1.0f;
		//影用
		if (GetOwnShadowActive()) {
			//Vec3 CalcLightDir = -1.0 * StageLight.m_Directional;
			//Vec3 LightAt = CameraPtr->GetAt();
			//Vec3 LightEye = CalcLightDir;
			//LightEye *= Shadowmap::GetLightHeight();
			//LightEye = LightAt + LightEye;
			//SmCb.LightPos = LightEye;
			//SmCb.LightPos.w = 1.0f;
			//Mat4x4 LightView, LightProj;
			////ライトのビューと射影を計算
			//LightView = XMMatrixLookAtLH(LightEye, LightAt, Vec3(0, 1.0f, 0));
			//LightProj = XMMatrixOrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
			//	Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
			//SmCb.LightView = transpose(LightView);
			//SmCb.LightProjection = transpose(LightProj);
		}
		//ボーンの設定
		size_t BoneSz = pImpl->m_LocalBonesMatrix.size();
		if (BoneSz > 0) {
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				Mat4x4 mat = pImpl->m_LocalBonesMatrix[b];
				mat.transpose();
				SmCb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
				SmCb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
				SmCb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
				cb_count += 3;
			}
		}
		else if (pImpl->m_MultiLocalBonesMatrix.size() > data.m_MultiMeshIndex) {
			//マルチメッシュのボーンがあった
			//ボーンの設定
			BoneSz = pImpl->m_MultiLocalBonesMatrix[data.m_MultiMeshIndex].size();
			if (BoneSz > 0) {
				UINT cb_count = 0;
				for (size_t b = 0; b < BoneSz; b++) {
					Mat4x4 mat = pImpl->m_MultiLocalBonesMatrix[data.m_MultiMeshIndex][b];
					mat.transpose();
					SmCb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
					SmCb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
					SmCb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
					cb_count += 3;
				}
			}
		}
		SmCb.ActiveFlg.y = CameraPtr->GetFar();
		SmCb.ActiveFlg.z = CameraPtr->GetNear();
	}

	bool AdvSmBaseDraw::IsOriginalMeshUse() const {
		return pImpl->m_UseOriginalMeshResource;
	}

	void AdvSmBaseDraw::SetOriginalMeshUse(bool b) {
		pImpl->m_UseOriginalMeshResource = b;
	}

	shared_ptr<MeshResource> AdvSmBaseDraw::GetOriginalMeshResource() const {
		return pImpl->m_OriginalMeshResource;
	}

	void AdvSmBaseDraw::SetOriginalMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_OriginalMeshResource = MeshRes;
	}

	shared_ptr<MeshResource> AdvSmBaseDraw::GetMeshResource() const {
		if (IsOriginalMeshUse()) {
			return pImpl->m_OriginalMeshResource;
		}
		else
		{
			auto shptr = pImpl->m_MeshResource.lock();
			if (shptr) {
				return shptr;
			}
		}
		return nullptr;
	}

	void AdvSmBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_MeshResource = MeshRes;
	}

	void AdvSmBaseDraw::SetMeshResource(const wstring& MeshKey) {
		this->SetMeshResource(App::GetApp()->GetResource<MeshResource>(MeshKey));
	}

	void AdvSmBaseDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_TextureResource = TextureRes;
	}

	void AdvSmBaseDraw::SetTextureResource(const wstring& TextureKey) {
		this->SetTextureResource(App::GetApp()->GetResource<TextureResource>(TextureKey));
	}

	shared_ptr<TextureResource> AdvSmBaseDraw::GetTextureResource() const {
		//テクスチャがなければnullを返す
		auto shptr = pImpl->m_TextureResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}

	Col4 AdvSmBaseDraw::GetEmissive() const {
		return pImpl->m_Emissive;
	}

	void AdvSmBaseDraw::SetEmissive(const Col4& col) {
		pImpl->m_Emissive = col;
	}

	Col4 AdvSmBaseDraw::GetDiffuse() const {
		return pImpl->m_Diffuse;
	}

	void AdvSmBaseDraw::SetDiffuse(const Col4& col) {
		pImpl->m_Diffuse = col;
	}

	Col4 AdvSmBaseDraw::GetSpecular() const {
		return pImpl->m_Specular;
	}

	void AdvSmBaseDraw::SetSpecular(const Col4& col) {
		pImpl->m_Specular = col;
	}

	bool AdvSmBaseDraw::GetOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}

	void AdvSmBaseDraw::SetOwnShadowActive(bool b) {
		pImpl->m_OwnShadowActive = b;
	}
}