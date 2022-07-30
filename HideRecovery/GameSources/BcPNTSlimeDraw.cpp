/*!
@file BcPNTSlimeDraw.cpp
@brief BcPNTSlimeDrawなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "BcPNTSlimeDraw.h"

#include "MaruUtility.h"
#include "MetaballChildrenRender.h"

#include "SlimeRenderTarget.h"
#include "GameStageBase.h"

namespace basecross {

	using namespace Metaball;

	//コンスタントバッファの実装
	IMPLEMENT_DX11_CONSTANT_BUFFER(SlimeBuffer)

	//コンスタントバッファに渡したいパラメータのみ記述する
	struct BcPNTSlimeDraw::Impl : public DrawObjectBase {
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

		BcPNTSlimeDraw::Impl() :
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

	BcPNTSlimeDraw::BcPNTSlimeDraw(const std::shared_ptr<GameObject>& objPtr)
		:BcBaseDraw(objPtr), pImpl(new Impl())
	{}

	void BcPNTSlimeDraw::OnCreate() {
		SetLightingEnabled(true);
		//マルチライトの設定
		for (int i = 0; i < GetMaxDirectionalLights(); i++) {
			SetLightEnabled(i, true);
		}
	}

	void BcPNTSlimeDraw::OnDraw() {

		if (!GetDrawActive()) {
			return;
		}

		if (!m_isDraw) {
			//return;
		}
		m_isDraw = false;

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

			//シェーダの設定
			if (IsPerPixelLighting()) {
				//ピクセルライティング
				//DrawStatic<VSPNTPoint, PSPNTPoint>(PtrMeshResource->GetMashData());
			}
			else {
				//頂点ライティング
				//DrawStatic<VSPNTPoint, PSPNTPoint>(PtrMeshResource->GetMashData());
			}
		}

		//下は現状使っていない
		return;

		//マルチメッシュリソースの取得
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					//シェーダの設定
					if (IsPerPixelLighting()) {
						//ピクセルライティング
						DrawStatic<SlimeVS, SlimePS>(vec[i]);
					}
					else {
						//頂点ライティング
						DrawStatic<SlimeVS, SlimePS>(vec[i]);
					}
				}
			}
		}

		//後始末
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}

	void BcPNTSlimeDraw::DrawStatic2(const MeshPrimData& data) {
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
		pD3D11DeviceContext->VSSetShader(SlimeVS::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(SlimeVS::GetPtr()->GetInputLayout());
		//ピクセルシェーダ
		pD3D11DeviceContext->PSSetShader(SlimePS::GetPtr()->GetShader(), nullptr, 0);
		
		//ライトを設定
		SetLightingParamaters(); //必要ないかもしれない
		//個別処理
		SlimeConstants slimeCb;  //使用するバッファによって変わる
		//コンスタントバッファの作成
		SetSlimeConstants(slimeCb, data);  //これそもそも書き換える
		//テクスチャ
		auto shTex = GetTextureResource();
		if (shTex) {
			//テクスチャがある
			slimeCb.activeFlg.y = 1;
		}
		else {
			//描画コンポーネントにはテクスチャがない
			if (shTex = data.m_TextureResource.lock()) {
				//テクスチャがある
				slimeCb.activeFlg.y = 1;
			}
			else {
				slimeCb.activeFlg.y = 0;
			}
		}
		//コンスタントバッファの更新
		//バッファの三洋は変わる
		pD3D11DeviceContext->UpdateSubresource(SlimeBuffer::GetPtr()->GetBuffer(), 0, nullptr, &slimeCb, 0, 0);
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = SlimeBuffer::GetPtr()->GetBuffer();
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
			pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
			//サンプラーを設定
			RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
			//法線マップ
			if (GetNormalMapTextureResource()) {
				if (IsOwnShadowActive()) {
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

		//SlimeRenderTarget
		//auto stage = GetGameStage();
		//std::shared_ptr<SlimeRenderTarget> slimeRenderTarget = stage->GetSlimeRenderTarget();
		//ID3D11ShaderResourceView* pSlimeRenderTarget = slimeRenderTarget->GetShaderResourceView();
		//pD3D11DeviceContext->PSSetShaderResources(1, 1, &pSlimeRenderTarget);

		//影とサンプラー
		if (IsOwnShadowActive()) {
			////シャドウマップのレンダラーターゲット
			//auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
			//ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
			//pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
			////シャドウマップサンプラー
			//ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
			//pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
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


		//後始末
		Dev->InitializeStates();
	}


	//引数が自分で生成したコンスタントバッファ用のコンストラクトに変える。
	void BcPNTSlimeDraw::SetSlimeConstants(SlimeConstants& pointCb, const MeshPrimData& data) {
		//行列の定義
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//カメラを得る
		auto CameraPtr = GetGameObject()->OnGetDrawCamera();
		//ライトを得る
		auto PtrLightObj = GetGameObject()->GetStage()->GetLight();
		auto PtrMultiLight = dynamic_pointer_cast<MultiLight>(PtrLightObj);
		if (PtrMultiLight) {
			//マルチライトだった
			pointCb.activeFlg.x = 3;
		}
		else {
			//そうではない
			pointCb.activeFlg.x = 1;
		}
		auto StageLight = GetGameObject()->OnGetDrawLight();
		//ワールド行列
		bsm::Mat4x4 world;
		if (data.m_UseMeshToTransformMatrix) {
			world = data.m_MeshToTransformMatrix * GetMeshToTransformMatrix();
			world *= PtrTrans->GetWorldMatrix();
		}
		else {
			world = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		}
		//ビュー行列
		bsm::Mat4x4 view = CameraPtr->GetViewMatrix();
		//射影行列
		bsm::Mat4x4 projection = CameraPtr->GetProjMatrix();

		//行列の設定
		auto worldView = world * view;
		pointCb.worldViewProj = XMMatrixTranspose(XMMatrixMultiply(worldView, projection));

		//pointCb.viewProj = XMMatrixTranspose(XMMatrixMultiply(view, projection));

		//フォグの設定
		if (IsFogEnabled())
		{
			auto start = GetFogStart();
			auto end = GetFogEnd();
			if (start == end)
			{
				// Degenerate case: force everything to 100% fogged if start and end are the same.
				static const XMVECTORF32 fullyFogged = { 0, 0, 0, 1 };
				pointCb.fogVector = fullyFogged;
			}
			else
			{
				XMMATRIX worldViewTrans = worldView;
				// _13, _23, _33, _43
				XMVECTOR worldViewZ = XMVectorMergeXY(XMVectorMergeZW(worldViewTrans.r[0], worldViewTrans.r[2]),
					XMVectorMergeZW(worldViewTrans.r[1], worldViewTrans.r[3]));
				XMVECTOR wOffset = XMVectorSwizzle<1, 2, 3, 0>(XMLoadFloat(&start));
				pointCb.fogVector = (worldViewZ + wOffset) / (start - end);
			}
			pointCb.fogColor = GetFogColor();
		}
		else
		{
			pointCb.fogVector = g_XMZero;
			pointCb.fogColor = g_XMZero;

		}

		//メタボールの描画
		int useNumSphereCount = 0;
		for (int i = 0; i < m_renders.size(); i++) {
			if (!m_renders[i]) {
				continue;
			}

			//表示するかどうか
			if(!IsMetaballDraw(m_renders[i]->GetPosition())) {
				continue;
			}

			pointCb.sphereDatas[useNumSphereCount].position = m_renders[i]->GetPosition();
			pointCb.sphereDatas[useNumSphereCount].radius = m_renders[i]->GetRadius();
			pointCb.sphereDatas[useNumSphereCount].color = m_renders[i]->GetColor();
			useNumSphereCount++;
		}

		pointCb.useNumSpheres = useNumSphereCount;

		//ライトの設定
		if (IsLightingEnabled())
		{
			//平行ライト
			if (pointCb.activeFlg.x == 1) {
				pointCb.lightDirection[0] = GetLightDirection(0);
				pointCb.lightDiffuseColor[0] = GetLightDiffuseColor(0);
				pointCb.lightSpecularColor[0] = GetLightSpecularColor(0);
			}
			else {
				for (int i = 0; i < GetMaxDirectionalLights(); i++) {
					if (IsLightEnabled(i)) {
						pointCb.lightDirection[i] = GetLightDirection(i);
						pointCb.lightDiffuseColor[i] = GetLightDiffuseColor(i);
						pointCb.lightSpecularColor[i] = GetLightSpecularColor(i);
					}
				}
			}

			//ワールド系の情報を入れる。
			pointCb.world = XMMatrixTranspose(world);

			XMMATRIX worldInverse = XMMatrixInverse(nullptr, world);

			pointCb.worldInverseTranspose[0] = worldInverse.r[0];
			pointCb.worldInverseTranspose[1] = worldInverse.r[1];
			pointCb.worldInverseTranspose[2] = worldInverse.r[2];

			XMMATRIX viewInverse = XMMatrixInverse(nullptr, view);

			pointCb.eyePosition = viewInverse.r[3];
		}
		XMVECTOR diffuse = GetDiffuse();
		XMVECTOR alphaVector = XMVectorReplicate(GetAlpha());
		XMVECTOR emissiveColor = GetEmissive();
		XMVECTOR ambientLightColor = GetAmbientLightColor();
		if (IsLightingEnabled())
		{
			// Merge emissive and ambient light contributions.
			pointCb.emissiveColor = (emissiveColor + ambientLightColor * diffuse) * alphaVector;
			pointCb.specularColorAndPower = GetSpecularColorAndPower();
		}
		else
		{
			pointCb.activeFlg.x = 0;
			// Merge diffuse and emissive light contributions.
			diffuse += emissiveColor;
			pointCb.specularColorAndPower = GetSpecularColorAndPower();
		}
		// xyz = diffuse * alpha, w = alpha.
		pointCb.diffuseColor = XMVectorSelect(alphaVector, diffuse * alphaVector, g_XMSelect1110);

		//影用
		if (GetOwnShadowActive()) {
			Vec3 CalcLightDir(StageLight.m_Directional * -1.0);
			Vec3 LightAt(CameraPtr->GetAt());
			Vec3 LightEye(CalcLightDir);
			LightEye *= Shadowmap::GetLightHeight();
			LightEye = LightAt + LightEye;
			Vec4 LightEye4(LightEye, 1.0f);
			LightEye4.w = 1.0f;
			pointCb.lightPos = LightEye4;
			Vec4 eyePos4(CameraPtr->GetEye(), 1.0f);
			eyePos4.w = 1.0f;
			pointCb.eyePos = eyePos4;
			Mat4x4 LightView, LightProj;
			//ライトのビューと射影を計算
			LightView = XMMatrixLookAtLH(LightEye, LightAt, bsm::Vec3(0, 1.0f, 0));
			LightProj = XMMatrixOrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
				Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
			pointCb.lightView = bsm::transpose(LightView);
			pointCb.lightProjection = bsm::transpose(LightProj);
		}
	}

	bool BcPNTSlimeDraw::IsMetaballDraw(const Vec3& targetPosition) {
		auto camera = GetStage()->GetView()->GetTargetCamera();
		auto cameraPosition = camera->GetEye();
		auto cameraForward = camera->GetAt() - camera->GetEye();
		auto toTargetVec = targetPosition - camera->GetEye();

		//正面方向にいなかったら描画しない
		if (!maru::Utility::IsTargetInFront(targetPosition, cameraPosition, cameraForward)) {
			return false;
		}

		//距離が離れているなら.....将来的に描画を落とす。
		//if (toTargetVec.length() > 10.0f) {
			
		//}

		return true;
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void BcPNTSlimeDraw::SetRenders(const vector<ex_weak_ptr<ChildrenRender>>& renders) {
		m_renders = renders;
	}

	void BcPNTSlimeDraw::AddRender(const std::shared_ptr<Metaball::ChildrenRender>& render) {
		m_renders.push_back(render);
	}
	void BcPNTSlimeDraw::RemoveRender(const ex_weak_ptr<Metaball::ChildrenRender>& render) {
		maru::Utility::RemoveVec(m_renders, render);
	}

	Vec3 BcPNTSlimeDraw::GetChildrenCenterPosition() const {
		int index = 0;
		Vec3 totalVec(0.0f);
		for (const auto& render : m_renders) {
			if (!render) {
				continue;
			}

			auto position = render->GetGameObject()->GetComponent<Transform>()->GetPosition();
			//auto position = render->GetGameObject()->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
			totalVec += position;
			index++;
		}

		return totalVec / (float)index;
		//return transform->GetWorldMatrix().transInMatrix();
	}

}

//endbasecross