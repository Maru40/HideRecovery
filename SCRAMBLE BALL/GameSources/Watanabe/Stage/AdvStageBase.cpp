﻿#include "stdafx.h"
#include "AdvStageBase.h"
#include "../Shader/PostProcess.h"

namespace basecross {
	IMPLEMENT_DX11_CONSTANT_BUFFER(AdvStageCB)

		shared_ptr<AdvRenderer> AdvStageBase::m_advRender;
	shared_ptr<PostProcess> AdvStageBase::m_postProcess;

	void AdvStageBase::UpdateStageCB() {
		AdvStageConstruct stageConstant;

		bsm::Mat4x4 viewMatrix, projectionMatrix, inverseViewMatrix, inverseProjMatrix;

		//カメラを得る
		auto camera = GetView()->GetTargetCamera();
		//ビューと射影行列を得る
		viewMatrix = camera->GetViewMatrix();
		inverseViewMatrix = viewMatrix;
		inverseViewMatrix.inverse();

		//転置する
		viewMatrix.transpose();
		//転置する
		projectionMatrix = camera->GetProjMatrix();
		inverseProjMatrix = projectionMatrix;
		inverseProjMatrix.inverse();

		projectionMatrix.transpose();
		stageConstant.view = viewMatrix;
		stageConstant.projection = projectionMatrix;
		stageConstant.inverseViewProj = inverseProjMatrix * inverseViewMatrix;
		stageConstant.inverseViewProj.transpose();

		stageConstant.viewPort = Vec2((float)App::GetApp()->GetGameWidth(), (float)App::GetApp()->GetGameHeight());
		stageConstant.viewNear = camera->GetNear();
		stageConstant.viewFar = camera->GetFar();

		stageConstant.lightAmbient = m_lightAmbient;

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		auto* constantBuffer = AdvStageCB::GetPtr()->GetBuffer();

		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(constantBuffer, 0, nullptr, &stageConstant, 0, 0);
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &constantBuffer);
		//コンピュートシェーダ(ポストエフェクト)用
		pD3D11DeviceContext->CSSetConstantBuffers(0, 1, &constantBuffer);
	}

	void AdvStageBase::DrawPostProcess() {
		GetPostProcess()->OnPostDraw();
	}

	void AdvStageBase::DrawStringSprite() {
		// StringSpriteは基本Debugでしか使わないのでDrawStage時に変数に入れたい
		for (auto& ptr : GetGameObjectVec()) {
			auto stringSprite = ptr->GetComponent<StringSprite>(false);

			if (stringSprite) {
				stringSprite->OnDraw();
			}
		}
	}

	const shared_ptr<AdvRenderer>& AdvStageBase::GetAdvRenderer() {
		if (!m_advRender) {
			m_advRender = make_shared<AdvRenderer>();
		}

		return m_advRender;
	}

	const shared_ptr<PostProcess>& AdvStageBase::GetPostProcess() {
		if (!m_postProcess) {
			m_postProcess = make_shared<PostProcess>();
		}
		return m_postProcess;
	}

	void AdvStageBase::RenderStage() {
		const auto& render = GetAdvRenderer();

		render->StartRenderTarget();
		render->RenderMain();
		Stage::DrawStage();

		//render->RenderPostProcessing();
		//UpdateStageCB();
		//DrawPostProcess();

		//render->RenderMain();
		//DrawSprite();

		render->PassRenderMain();

		// StringSpriteは現在のレンダーターゲットに書き込まれるため
		// 最後に描画
		DrawStringSprite();
	}

	void AdvStageBase::DrawStage() {
		//レイヤーの取得と設定
		set<int> DrawLayers;
		//Spriteかそうでないかを分離
		for (auto& ptr : GetGameObjectVec()) {
			if (ptr->IsDrawActive()) {
				//描画レイヤーに登録
				DrawLayers.insert(ptr->GetDrawLayer());
				//Spriteかその派生クラスなら分離
				if (ptr->GetComponent<SpriteBaseDraw>(false) || ptr->IsSpriteDraw()) {
					m_spriteVec.push_back(ptr);
				}
				else {
					m_object3DVec.push_back(ptr);
				}
			}
		}
		//3Dの透明と非透明を分離
		for (auto& ptr : m_object3DVec) {
			if (ptr->IsDrawActive()) {
				if (ptr->IsAlphaActive()) {
					m_object3DAlphaVec.push_back(ptr);
				}
				else {
					m_object3DNormalVec.push_back(ptr);
				}
			}
		}

		auto PtrCamera = GetView()->GetTargetCamera();
		//カメラの位置
		bsm::Vec3 CameraEye = PtrCamera->GetEye();
		//透明の3Dオブジェクトをカメラからの距離でソート
		//以下は、オブジェクトを引数に取りboolを返すラムダ式
		//--------------------------------------------------------
		auto func = [&](shared_ptr<GameObject>& Left, shared_ptr<GameObject>& Right)->bool {
			auto PtrLeftTrans = Left->GetComponent<Transform>();
			auto PtrRightTrans = Right->GetComponent<Transform>();

			auto LeftPos = PtrLeftTrans->GetWorldMatrix().transInMatrix();
			auto RightPos = PtrRightTrans->GetWorldMatrix().transInMatrix();

			auto LeftLen = bsm::length(LeftPos - CameraEye);
			auto RightLen = bsm::length(RightPos - CameraEye);

			return (LeftLen > RightLen);
		};

		std::sort(m_object3DAlphaVec.begin(), m_object3DAlphaVec.end(), func);

		//3Dノーマルオブジェクトの描画準備
		for (auto& ptr : m_object3DNormalVec) {
			ptr->OnPreDraw();
		}
		//3D透明オブジェクトの描画準備
		for (auto& ptr : m_object3DAlphaVec) {
			ptr->OnPreDraw();
		}
		//パーティクルの描画準備（透明）
		GetParticleManager(false)->OnPreDraw();
		//パーティクルの描画準備（加算）
		GetParticleManager(true)->OnPreDraw();
		//スプライトオブジェクトの描画準備
		for (auto& ptr : m_spriteVec) {
			ptr->OnPreDraw();
		}

		//--------------------------------------------------------
		//スプライトをZ座標距離でソート
		//以下は、オブジェクトを引数に取りboolを返すラムダ式
		//--------------------------------------------------------
		auto funcSprite = [&](shared_ptr<GameObject>& Left, shared_ptr<GameObject>& Right)->bool {
			auto PtrLeftTrans = Left->GetComponent<Transform>();
			auto PtrRightTrans = Right->GetComponent<Transform>();

			auto LeftPos = PtrLeftTrans->GetWorldMatrix().transInMatrix();
			auto RightPos = PtrRightTrans->GetWorldMatrix().transInMatrix();

			float LeftZ = LeftPos.z;
			float RightZ = RightPos.z;
			return (LeftZ > RightZ);
		};
		std::sort(m_spriteVec.begin(), m_spriteVec.end(), funcSprite);

		for (auto it = DrawLayers.begin(); it != DrawLayers.end(); it++) {
			int Tgt = *it;
			//3Dノーマルオブジェクトの描画
			for (auto& ptr : m_object3DNormalVec) {
				if (ptr->GetDrawLayer() == Tgt) {
					ptr->OnDraw();
				}
			}
			//3D透明オブジェクトの描画
			for (auto& ptr : m_object3DAlphaVec) {
				if (ptr->GetDrawLayer() == Tgt) {
					ptr->OnDraw();
				}
			}
			//パーティクルの描画
			//パーティクルマネージャは描画レイヤーごとに初期化されるので
			//毎レイヤー描画する
			//透明処理
			GetParticleManager(false)->OnDraw();
			//加算処理
			GetParticleManager(true)->OnDraw();
			//スプライトオブジェクトの描画
			for (auto& ptr : m_spriteVec) {
				if (ptr->GetDrawLayer() == Tgt) {
					ptr->OnDraw();
				}
			}
		}
		//ステージのDraw();
		OnDraw();
		//ワーク用配列のクリア
		//ワーク配列は毎ターンごとに初期化されるが、
		//最大値は減らないので2回目のターン以降は高速に動作する
		m_object3DVec.clear();
		m_spriteVec.clear();
		m_object3DNormalVec.clear();
		m_object3DAlphaVec.clear();
	}

	void AdvStageBase::DrawSprite() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		ID3D11UnorderedAccessView* pNull[] = { nullptr };
		pD3D11DeviceContext->CSSetUnorderedAccessViews(0, _countof(pNull), pNull, nullptr);

		//スプライトオブジェクトの描画準備
		for (auto& ptr : m_spriteVec) {
			ptr->OnPreDraw();
		}

		//--------------------------------------------------------
		//スプライトをZ座標距離でソート
		//以下は、オブジェクトを引数に取りboolを返すラムダ式
		//--------------------------------------------------------
		auto funcSprite = [&](shared_ptr<GameObject>& Left, shared_ptr<GameObject>& Right)->bool {
			auto PtrLeftTrans = Left->GetComponent<Transform>();
			auto PtrRightTrans = Right->GetComponent<Transform>();

			auto LeftPos = PtrLeftTrans->GetWorldMatrix().transInMatrix();
			auto RightPos = PtrRightTrans->GetWorldMatrix().transInMatrix();

			float LeftZ = LeftPos.z;
			float RightZ = RightPos.z;
			return (LeftZ > RightZ);
		};

		std::sort(m_spriteVec.begin(), m_spriteVec.end(), funcSprite);

		for (auto it = m_drawLayers.begin(); it != m_drawLayers.end(); it++) {
			int Tgt = *it;
			//スプライトオブジェクトの描画
			for (auto& ptr : m_spriteVec) {
				if (ptr->GetDrawLayer() == Tgt) {
					ptr->OnDraw();
				}
			}
		}

		m_spriteVec.clear();
		m_drawLayers.clear();
	}
}