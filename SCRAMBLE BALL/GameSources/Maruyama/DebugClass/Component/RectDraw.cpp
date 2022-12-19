
/*!
@file RectDraw.cpp
@brief RectDrawのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "RectDraw.h"

#include "Maruyama/UI/BuilderVertexPCT.h"

namespace basecross {

	RectDraw::RectDraw(
		const std::shared_ptr<GameObject>& objPtr, 
		const maru::Rect& rect, 
		const std::wstring& texture,
		const Col4& color
	):
		Component(objPtr),
		m_rect(rect),
		m_texture(texture),
		m_color(color)
	{}

	void RectDraw::OnCreate() {
		m_selfTransform = GetGameObject()->GetComponent<Transform>();

		CreateRectObject();
	}

	void RectDraw::OnUpdate() {
		if (auto rectObjectTransform = m_rectObjectTransform.lock()) {
			rectObjectTransform->SetPosition(m_selfTransform.lock()->GetPosition());
			rectObjectTransform->SetForward(m_selfTransform.lock()->GetForward());
			rectObjectTransform->SetScale(m_selfTransform.lock()->GetScale());
		}
	}

	void RectDraw::CreateRectObject() {
		//オブジェクト生成
		auto object = GetStage()->Instantiate<GameObject>(m_rect.centerPosition, Quat::Identity());
		object->SetAlphaActive(true);
		
		//頂点とインデックスデータの生成
		constexpr float fScale = 0.8f;
		auto buildParam = Builder::VertexPCTParametor(Vec3(fScale), Vec2(512.0f, 256.0f), m_texture);	//正方形用のセルを設定	//Arrow_TX
		buildParam.color = m_color;
		auto data = Builder::BuilderVertexPCT(buildParam);

		//表示コンポーネントのアタッチ
		auto draw = object->AddComponent<PCTStaticDraw>();
		draw->SetOriginalMeshUse(true);
		draw->CreateOriginalMesh(data.m_vertices, data.m_indices);
		draw->SetDepthStencilState(DepthStencilState::Read);
		
		//テクスチャがあるならテクスチャを張る
		if (buildParam.texture != L"") {
			draw->SetTextureResource(buildParam.texture);
		}

		//大きさ調整
		const auto& width = m_rect.width;
		constexpr float Height = 1.0f;
		const auto& depth = m_rect.depth;
		auto objTrans = object->GetComponent<Transform>();

		objTrans->SetScale(Vec3(width, Height, depth));

		m_rectObject = object;
		m_rectObjectTransform = objTrans;
	}

}