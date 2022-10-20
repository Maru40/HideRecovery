/*!
@file   AdvBaseDraw.cpp
@brief  カスタム描画クラス実体
*/

#include "stdafx.h"
#include "AdvBaseDraw.h"
#include "DrawComponents.h"

namespace basecross {
	struct AdvBaseDraw::Impl :public DrawObjectBase {
		//頂点変更する場合のメッシュ（オリジナル）
		shared_ptr<MeshResource> m_OriginalMeshResource;
		//オリジナルメッシュを使うかどうか
		bool m_UseOriginalMeshResource;
		//テクスチャリソース
		vector<weak_ptr<TextureResource>> m_TextureResources;
		//エミッシブ色
		Col4 m_Emissive;
		// デフューズ色
		Col4 m_Diffuse;
		// スペキュラー色
		Col4 m_Specular;
		//影を投影するかどうか
		bool m_OwnShadowActive;
		//モデルに入っているDiffuseを使うかどうか
		bool m_ModelDiffusePriority;
		//モデルに入っているEmissiveを使うかどうか
		bool m_ModelEmissivePriority;
		//モデルに入っているテクスチャを使うかどうか
		bool m_ModelTextureEnabled;
		////Instance描画用
		////Instance最大値
		//size_t m_MaxInstance;
		///// 行列用の頂点バッファ
		//ComPtr<ID3D11Buffer> m_MatrixBuffer;
		/////行列の配列
		//vector<Mat4x4> m_MatrixVec;
		////行列を自動クリアするかどうか
		//bool m_AutoClearMatrixVec;
		AdvBaseDraw::Impl() :
			m_UseOriginalMeshResource(false),
			m_Emissive(0, 0, 0, 0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_Specular(0, 0, 0, 0),
			m_OwnShadowActive(false),
			m_ModelDiffusePriority(false),
			m_ModelEmissivePriority(false),
			m_ModelTextureEnabled(true)
			//m_MaxInstance(2000),
			//m_AutoClearMatrixVec(false)
		{}
	};

	AdvBaseDraw::AdvBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	AdvBaseDraw::~AdvBaseDraw() {}

	void AdvBaseDraw::SetConstants(AdvConstants& SmCb, const MeshPrimData& data) {
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
		//ライティング
		auto StageLight = GetGameObject()->OnGetDrawLight();
		SmCb.LightDir = StageLight.m_Directional;
		SmCb.LightDir.w = 1.0f;
		SmCb.EyePos = CameraPtr->GetEye();
		SmCb.EyePos.w = 1.0f;
		//影用
		if (GetOwnShadowActive()) {
			Vec3 CalcLightDir = -1.0 * StageLight.m_Directional;
			Vec3 LightAt = CameraPtr->GetAt();
			Vec3 LightEye = CalcLightDir;
			LightEye *= Shadowmap::GetLightHeight();
			LightEye = LightAt + LightEye;
			SmCb.LightPos = LightEye;
			SmCb.LightPos.w = 1.0f;
			Mat4x4 LightView, LightProj;
			//ライトのビューと射影を計算
			LightView = XMMatrixLookAtLH(LightEye, LightAt, Vec3(0, 1.0f, 0));
			LightProj = XMMatrixOrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
				Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
			SmCb.LightView = transpose(LightView);
			SmCb.LightProjection = transpose(LightProj);
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
	}

#pragma region Instance
	////行列バッファの作成
	//void AdvBaseDraw::CreateMatrixBuffer() {
	//	//インスタンス行列バッファの作成
	//	//Max値で作成する
	//	vector<Mat4x4> matrices(pImpl->m_MaxInstance);
	//	for (auto& m : matrices) {
	//		m = Mat4x4();
	//	}
	//	MeshResource::CreateDynamicVertexBuffer(pImpl->m_MatrixBuffer, matrices);
	//}

	////行列バッファのマップ
	//void AdvBaseDraw::MapMatrixBuffer() {
	//	//デバイスの取得
	//	auto Dev = App::GetApp()->GetDeviceResources();
	//	auto pDx11Device = Dev->GetD3DDevice();
	//	auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
	//	//インスタンスバッファにマップ
	//	D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
	//	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	//	//行列のマップ
	//	if (FAILED(pID3D11DeviceContext->Map(pImpl->m_MatrixBuffer.Get(), 0, mapType, 0, &mappedBuffer))) {
	//		// Map失敗
	//		throw BaseException(
	//			L"行列のMapに失敗しました。",
	//			L"if(FAILED(pID3D11DeviceContext->Map()))",
	//			L"AdvBaseDraw::MapMatrixBuffer()"
	//		);
	//	}
	//	//行列の変更
	//	auto* matrices = (Mat4x4*)mappedBuffer.pData;
	//	Mat4x4 World;
	//	for (size_t i = 0; i < pImpl->m_MatrixVec.size(); i++) {
	//		//ワールド行列
	//		World = pImpl->m_MatrixVec[i];
	//		//転置する
	//		World.transpose();
	//		matrices[i] = World;
	//	}
	//	//アンマップ
	//	pID3D11DeviceContext->Unmap(pImpl->m_MatrixBuffer.Get(), 0);
	//}
#pragma endregion

	bool AdvBaseDraw::IsOriginalMeshUse() const {
		return pImpl->m_UseOriginalMeshResource;
	}

	void AdvBaseDraw::SetOriginalMeshUse(bool b) {
		pImpl->m_UseOriginalMeshResource = b;
	}

	shared_ptr<MeshResource> AdvBaseDraw::GetOriginalMeshResource() const {
		return pImpl->m_OriginalMeshResource;
	}

	void AdvBaseDraw::SetOriginalMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_OriginalMeshResource = MeshRes;
	}

	shared_ptr<MeshResource> AdvBaseDraw::GetMeshResource() const {
		if (IsOriginalMeshUse()) {
			return pImpl->m_OriginalMeshResource;
		}
		else {
			auto shptr = pImpl->m_MeshResource.lock();
			if (shptr) {
				return shptr;
			}
		}
		return nullptr;
	}

	void AdvBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_MeshResource = MeshRes;
	}

	void AdvBaseDraw::SetMeshResource(const wstring& MeshKey) {
		this->SetMeshResource(App::GetApp()->GetResource<MeshResource>(MeshKey));
	}

	void AdvBaseDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes, size_t index) {
		if (pImpl->m_TextureResources.size() >= index) {
			pImpl->m_TextureResources.push_back(TextureRes);
		}
		else {
			pImpl->m_TextureResources[index] = TextureRes;
		}
	}

	void AdvBaseDraw::SetTextureResource(const wstring& TextureKey, size_t index) {
		this->SetTextureResource(App::GetApp()->GetResource<TextureResource>(TextureKey), index);
	}

	shared_ptr<TextureResource> AdvBaseDraw::GetTextureResource(size_t index) const {
		// インデックスが要素より大きければnullを返す
		if (pImpl->m_TextureResources.size() <= index) {
			return nullptr;
		}

		//テクスチャがなければnullを返す
		auto shptr = pImpl->m_TextureResources[index].lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}

	vector<weak_ptr<TextureResource>> AdvBaseDraw::GetAllTextureResource() const {
		return pImpl->m_TextureResources;
	}

	shared_ptr<MultiMeshResource> AdvBaseDraw::GetMultiMeshResource() const {
		//なければnullを返す
		auto shptr = pImpl->m_MultiMeshResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}

	void AdvBaseDraw::SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr) {
		pImpl->m_MultiMeshResource = MeshResourcePtr;
		pImpl->m_MultiMeshDrawVec.clear();
		for (size_t i = 0; i < MeshResourcePtr->GetMeshVecCount(); i++) {
			//最初はすべてのマルチメッシュを描画
			pImpl->m_MultiMeshDrawVec.push_back(true);
		}
	}

	void AdvBaseDraw::SetMultiMeshResource(const wstring& ResKey) {
		this->SetMultiMeshResource(App::GetApp()->GetResource<MultiMeshResource>(ResKey));
	}

	bool AdvBaseDraw::GetMultiMeshIsDraw(size_t index) const {
		if (index >= pImpl->m_MultiMeshDrawVec.size()) {
			throw BaseException(
				L"インデックスがマルチメッシュのメッシュ数を超えてます",
				L"if (index >= pImpl->m_BcDrawObject.m_MultiMeshDrawVec.size())",
				L"AdvBaseDraw::GetMultiMeshIsDraw()"
			);
		}
		return pImpl->m_MultiMeshDrawVec[index];
	}
	void AdvBaseDraw::SetMultiMeshIsDraw(size_t index, bool b) {
		if (index >= pImpl->m_MultiMeshDrawVec.size()) {
			throw BaseException(
				L"インデックスがマルチメッシュのメッシュ数を超えてます",
				L"if (index >= pImpl->m_BcDrawObject.m_MultiMeshDrawVec.size())",
				L"AdvBaseDraw::SetMultiMeshIsDraw()"
			);
		}
		pImpl->m_MultiMeshDrawVec[index] = b;
	}

	Col4 AdvBaseDraw::GetEmissive() const {
		return pImpl->m_Emissive;
	}

	void AdvBaseDraw::SetEmissive(const Col4& col) {
		pImpl->m_Emissive = col;
	}

	Col4 AdvBaseDraw::GetDiffuse() const {
		return pImpl->m_Diffuse;
	}

	void AdvBaseDraw::SetDiffuse(const Col4& col) {
		pImpl->m_Diffuse = col;
	}

	Col4 AdvBaseDraw::GetSpecular() const {
		return pImpl->m_Specular;
	}

	void AdvBaseDraw::SetSpecular(const Col4& col) {
		pImpl->m_Specular = col;
	}

	bool AdvBaseDraw::GetOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}

	bool AdvBaseDraw::IsOwnShadowActive() const {
		return pImpl->m_OwnShadowActive;
	}

	void AdvBaseDraw::SetOwnShadowActive(bool b) {
		pImpl->m_OwnShadowActive = b;
	}

	bool AdvBaseDraw::GetModelDiffusePriority() const {
		return pImpl->m_ModelDiffusePriority;
	}

	bool AdvBaseDraw::IsModelDiffusePriority() const {
		return pImpl->m_ModelDiffusePriority;
	}

	void AdvBaseDraw::SetModelDiffusePriority(bool b) {
		pImpl->m_ModelDiffusePriority = b;
	}

	bool AdvBaseDraw::GetModelEmissivePriority() const {
		return pImpl->m_ModelEmissivePriority;
	}

	bool AdvBaseDraw::IsModelEmissivePriority() const {
		return pImpl->m_ModelEmissivePriority;
	}

	void AdvBaseDraw::SetModelEmissivePriority(bool b) {
		pImpl->m_ModelEmissivePriority = b;
	}

	bool AdvBaseDraw::GetModelTextureEnabled() const {
		return pImpl->m_ModelTextureEnabled;
	}

	bool AdvBaseDraw::IsModelTextureEnabled() const {
		return pImpl->m_ModelTextureEnabled;
	}

	void AdvBaseDraw::SetModelTextureEnabled(bool b) {
		pImpl->m_ModelTextureEnabled = b;
	}

	void AdvBaseDraw::BoneInit() {
		pImpl->BoneInit();
	}

	void AdvBaseDraw::InterpolationMatrix(const Mat4x4& m1, const Mat4x4& m2, float t, Mat4x4& out) {
		pImpl->InterpolationMatrix(m1, m2, t, out);
	}

	void AdvBaseDraw::AddAnimation(const wstring& Name, int StartSample, int SampleLength, bool Loop,
		float SamplesParSecond) {
		pImpl->AddAnimation(Name, StartSample, SampleLength, Loop, SamplesParSecond);
	}

	void AdvBaseDraw::ChangeCurrentAnimation(const wstring& AnemationName, float StartTime) {
		pImpl->ChangeCurrentAnimation(AnemationName, StartTime);
	}

	const wstring& AdvBaseDraw::GetCurrentAnimation() const {
		return pImpl->m_CurrentAnimeName;
	}

	float AdvBaseDraw::GetCurrentAnimationTime() const {
		return pImpl->m_CurrentAnimeTime;
	}

	bool AdvBaseDraw::IsTargetAnimeEnd() const {
		auto& AnimData = pImpl->GetAnimationData();
		return AnimData.m_IsAnimeEnd;
	}

	bool AdvBaseDraw::UpdateAnimation(float ElapsedTime) {
		return pImpl->UpdateAnimation(ElapsedTime);
	}

	const vector< Mat4x4 >* AdvBaseDraw::GetVecLocalBonesPtr() const {
		return &pImpl->m_LocalBonesMatrix;
	}

	vector< Mat4x4 >& AdvBaseDraw::GetVecLocalBones() {
		return pImpl->m_LocalBonesMatrix;
	}

	const vector< Mat4x4 >* AdvBaseDraw::GetVecMultiLocalBonesPtr(size_t index) const {
		if (pImpl->m_MultiLocalBonesMatrix.size() > index) {
			return &(pImpl->m_MultiLocalBonesMatrix[index]);
		}
		return nullptr;
	}

	vector< Mat4x4 >& AdvBaseDraw::GetVecMultiLocalBones(size_t index) {
		if (pImpl->m_MultiLocalBonesMatrix.size() <= index) {
			throw BaseException(
				L"インデックスが上限を超えてます",
				L"if (pImpl->m_MultiLocalBonesMatrix.size() <= index)",
				L"AdvBaseDraw::GetVecMultiLocalBones"
			);
		}
		return pImpl->m_MultiLocalBonesMatrix[index];
	}

#pragma region Instance
	//size_t AdvBaseDraw::GetMaxInstance() const {
	//	return pImpl->m_MaxInstance;
	//}

	//void AdvBaseDraw::ResizeMaxInstance(size_t NewSize) {
	//	pImpl->m_MaxInstance = NewSize;
	//	CreateMatrixBuffer();
	//}

	//void AdvBaseDraw::AddMatrix(const Mat4x4& NewMat) {
	//	if (pImpl->m_MatrixVec.size() >= GetMaxInstance()) {
	//		throw BaseException(
	//			L"インスタンス上限を超えてます",
	//			L"if (pImpl->m_MatrixVec.size() >= GetMaxInstance())",
	//			L"AdvBaseDraw::AddMatrix()"
	//		);
	//	}
	//	pImpl->m_MatrixVec.push_back(NewMat);
	//}

	//void AdvBaseDraw::UpdateMultiMatrix(const vector<Mat4x4>& NewMatVec) {
	//	pImpl->m_MatrixVec.resize(NewMatVec.size());
	//	pImpl->m_MatrixVec = NewMatVec;
	//}

	//vector<Mat4x4>& AdvBaseDraw::GetMatrixVec() const {
	//	return pImpl->m_MatrixVec;
	//}

	//void AdvBaseDraw::ClearMatrixVec() {
	//	pImpl->m_MatrixVec.clear();
	//}

	//bool AdvBaseDraw::IsAutoClearMatrixVec() const {
	//	return pImpl->m_AutoClearMatrixVec;
	//}
	//void AdvBaseDraw::SetAutoClearMatrixVec(bool b) {
	//	pImpl->m_AutoClearMatrixVec = b;
	//}

	//ComPtr<ID3D11Buffer>& AdvBaseDraw::GetMatrixBuffer() const {
	//	return pImpl->m_MatrixBuffer;
	//}
#pragma endregion

	void AdvBaseDraw::GetStaticMeshLocalPositions(vector<Vec3>& vertices) {
		auto ReshRes = GetMeshResource();
		if (!ReshRes) {
			throw BaseException(
				L"メッシュリソースがありません",
				L"if (!ReshRes)",
				L"AdvBaseDraw::GetStaticMeshLocalPositions()"
			);
		}
		vertices.clear();
		ReshRes->GetLocalPositions(vertices);
	}

	void AdvBaseDraw::GetStaticMeshWorldPositions(vector<Vec3>& vertices) {
		GetStaticMeshLocalPositions(vertices);
		//ワールド行列の反映
		auto WorldMat = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
		for (auto& v : vertices) {
			v *= WorldMat;
		}
	}

	bool AdvBaseDraw::HitTestStaticMeshSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, Vec3& HitPoint,
		TRIANGLE& RetTri, size_t& RetIndex) {
		GetStaticMeshWorldPositions(pImpl->m_TempPositions);
		for (size_t i = 0; i < pImpl->m_TempPositions.size(); i += 3) {
			TRIANGLE tri;
			tri.m_A = pImpl->m_TempPositions[i];
			tri.m_B = pImpl->m_TempPositions[i + 1];
			tri.m_C = pImpl->m_TempPositions[i + 2];
			if (!tri.IsValid()) {
				//三角形が無効なら次にうつる
				continue;
			}
			Vec3 ret;
			float t;
			if (HitTest::SEGMENT_TRIANGLE(StartPos, EndPos, tri, ret, t)) {
				auto Len = length(EndPos - StartPos);
				Len *= t;
				auto Nomal = EndPos - StartPos;
				Nomal.normalize();
				Nomal *= Len;
				HitPoint = StartPos + Nomal;
				RetTri = tri;
				RetIndex = i / 3;
				return true;
			}
		}
		return false;
	}

	bool AdvBaseDraw::HitTestStaticMeshSphereTriangles(const SPHERE& StartSp, const SPHERE& EndSp, Vec3& HitPoint, TRIANGLE& RetTri, size_t& RetIndex) {
		GetStaticMeshWorldPositions(pImpl->m_TempPositions);
		for (size_t i = 0; i < pImpl->m_TempPositions.size(); i += 3) {
			TRIANGLE tri;
			tri.m_A = pImpl->m_TempPositions[i];
			tri.m_B = pImpl->m_TempPositions[i + 1];
			tri.m_C = pImpl->m_TempPositions[i + 2];
			if (!tri.IsValid()) {
				//三角形が無効なら次にうつる
				continue;
			}
			Vec3 ret;
			float t;
			//球体の移動はStartSpからEndSpに1.0とする
			Vec3 Valocity = EndSp.m_Center - StartSp.m_Center;
			if (HitTest::CollisionTestSphereTriangle(StartSp, Valocity, tri, 0.0f, 1.0f, t)) {
				auto Len = length(EndSp.m_Center - StartSp.m_Center);
				Len *= t;
				auto Nomal = EndSp.m_Center - StartSp.m_Center;
				Nomal.normalize();
				Nomal *= Len;
				HitPoint = StartSp.m_Center + Nomal;
				RetTri = tri;
				RetIndex = i / 3;
				return true;
			}
		}
		return false;
	}

	void AdvBaseDraw::GetSkinedMeshLocalPositions(vector<Vec3>& vertices) {
		if (GetVecLocalBones().size() == 0) {
			throw BaseException(
				L"ボーン行列がありません",
				L"if (GetVecLocalBones().size() == 0)",
				L"AdvBaseDraw::GetSkinedMeshLocalPositions()"
			);
		}
		auto ReshRes = GetMeshResource();
		if (!ReshRes) {
			throw BaseException(
				L"メッシュリソースがありません",
				L"if (!ReshRes)",
				L"AdvBaseDraw::GetSkinedMeshLocalPositions()"
			);
		}
		vertices.clear();
		auto& Bones = GetVecLocalBones();
		auto& PosVec = ReshRes->GetVerteces();
		auto& SkinVec = ReshRes->GetSkins();
		for (auto& v : PosVec) {
			vertices.push_back(v.position);
		}
		//スキニング処理
		for (size_t j = 0; j < vertices.size(); j++) {
			Mat4x4 skinning(0);
			for (size_t i = 0; i < 4; i++)
			{
				skinning += Bones[SkinVec[j].indices[i]] * SkinVec[j].weights[i];
			}
			skinning._14 = 1.0f;
			skinning._24 = 1.0f;
			skinning._34 = 1.0f;
			skinning._44 = 1.0f;
			Vec4 p(vertices[j]);
			p.w = 1.0f;
			p *= skinning;
			vertices[j] = p;
		}
	}

	void AdvBaseDraw::GetSkinedMeshWorldPositions(vector<Vec3>& vertices) {
		GetSkinedMeshLocalPositions(vertices);
		//ワールド行列の反映
		auto WorldMat = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
		for (auto& v : vertices) {
			v *= WorldMat;
		}
	}

	bool AdvBaseDraw::HitTestSkinedMeshSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos,
		Vec3& HitPoint, TRIANGLE& RetTri, size_t& RetIndex) {
		GetSkinedMeshWorldPositions(pImpl->m_TempPositions);
		for (size_t i = 0; i < pImpl->m_TempPositions.size(); i += 3) {
			TRIANGLE tri;
			tri.m_A = pImpl->m_TempPositions[i];
			tri.m_B = pImpl->m_TempPositions[i + 1];
			tri.m_C = pImpl->m_TempPositions[i + 2];
			if (!tri.IsValid()) {
				//三角形が無効なら次にうつる
				continue;
			}
			Vec3 ret;
			float t;
			if (HitTest::SEGMENT_TRIANGLE(StartPos, EndPos, tri, ret, t)) {
				auto Len = length(EndPos - StartPos);
				Len *= t;
				auto Nomal = EndPos - StartPos;
				Nomal.normalize();
				Nomal *= Len;
				HitPoint = StartPos + Nomal;
				RetTri = tri;
				RetIndex = i / 3;
				return true;
			}
		}
		return false;
	}

	bool AdvBaseDraw::HitTestSkinedMeshSphereTriangles(const SPHERE& StartSp, const SPHERE& EndSp,
		Vec3& HitPoint, TRIANGLE& RetTri, size_t& RetIndex) {
		GetSkinedMeshWorldPositions(pImpl->m_TempPositions);
		for (size_t i = 0; i < pImpl->m_TempPositions.size(); i += 3) {
			TRIANGLE tri;
			tri.m_A = pImpl->m_TempPositions[i];
			tri.m_B = pImpl->m_TempPositions[i + 1];
			tri.m_C = pImpl->m_TempPositions[i + 2];
			if (!tri.IsValid()) {
				//三角形が無効なら次にうつる
				continue;
			}
			Vec3 ret;
			float t;
			//球体の移動はStartSpからEndSpに1.0とする
			Vec3 Valocity = EndSp.m_Center - StartSp.m_Center;
			if (HitTest::CollisionTestSphereTriangle(StartSp, Valocity, tri, 0.0f, 1.0f, t)) {
				auto Len = length(EndSp.m_Center - StartSp.m_Center);
				Len *= t;
				auto Nomal = EndSp.m_Center - StartSp.m_Center;
				Nomal.normalize();
				Nomal *= Len;
				HitPoint = StartSp.m_Center + Nomal;
				RetTri = tri;
				RetIndex = i / 3;
				return true;
			}
		}
		return false;
	}
}