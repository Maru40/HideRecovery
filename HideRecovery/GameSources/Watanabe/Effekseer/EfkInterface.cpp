/*!
@file EfkInterface.cpp
@brief Effekseerエフェクトのインターフェイス実体
*/

#include "stdafx.h"
#include "EfkEffect.h"

namespace basecross {
	// 静的メンバ変数の実体
	shared_ptr<EfkInterface> EfkInterface::m_ownInstance = nullptr;

	EfkInterface::EfkInterface(const shared_ptr<Stage>& stage)
		:BaseSingletonGameObject(stage),
		m_Manager(nullptr),
		m_Renderer(nullptr)
	{}
	EfkInterface::~EfkInterface() {}

	void EfkInterface::OnCreate() {
		CreateInstance();

		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		// 描画用インスタンスの生成
		m_Renderer = ::EffekseerRendererDX11::Renderer::Create(pDx11Device, pID3D11DeviceContext, 2000);
		// エフェクト管理用インスタンスの生成
		m_Manager = ::Effekseer::Manager::Create(2000);

		// 描画用インスタンスから描画機能を設定
		m_Manager->SetSpriteRenderer(m_Renderer->CreateSpriteRenderer());
		m_Manager->SetRibbonRenderer(m_Renderer->CreateRibbonRenderer());
		m_Manager->SetRingRenderer(m_Renderer->CreateRingRenderer());
		m_Manager->SetTrackRenderer(m_Renderer->CreateTrackRenderer());
		m_Manager->SetModelRenderer(m_Renderer->CreateModelRenderer());

		// 描画用インスタンスからテクスチャの読込機能を設定
		// 独自拡張可能、現在はファイルから読み込んでいる。
		m_Manager->SetTextureLoader(m_Renderer->CreateTextureLoader());
		m_Manager->SetModelLoader(m_Renderer->CreateModelLoader());
		m_Manager->SetMaterialLoader(m_Renderer->CreateMaterialLoader());
		m_Manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
		// レイヤーを前面に（要修正）
		SetDrawLayer(2);
	}

	void EfkInterface::OnUpdate() {
		// エフェクトの更新処理を行う
		m_Manager->Update();
	}

	void EfkInterface::OnDraw() {
		// ビューと射影行列を設定
		auto& camera = GetStage()->GetView()->GetTargetCamera();
		SetViewProj(camera->GetViewMatrix(), camera->GetProjMatrix());

		// エフェクトの描画開始処理を行う。
		m_Renderer->BeginRendering();

		// エフェクトの描画を行う。
		m_Manager->Draw();

		// エフェクトの描画終了処理を行う。
		m_Renderer->EndRendering();
	}

	void Mat4x4ToMatrix44(const bsm::Mat4x4& src, Effekseer::Matrix44& dest) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				dest.Values[i][j] = src(i, j);
			}
		}
	}

	void EfkInterface::SetViewProj(const bsm::Mat4x4& view, const bsm::Mat4x4& proj) {
		Effekseer::Matrix44 v, p;
		Mat4x4ToMatrix44(view, v);
		Mat4x4ToMatrix44(proj, p);
		m_Renderer->SetCameraMatrix(v);
		m_Renderer->SetProjectionMatrix(p);
	}

	void EfkInterface::SetTimeScale(float scale) {
		//m_Manager->SetGroupMask
	}
}
// end basecross