
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

	RectDraw::RectDraw(const std::shared_ptr<GameObject>& objPtr, const maru::Rect& rect):
		Component(objPtr),
		m_rect(rect)
	{}

	void RectDraw::OnCreate() {
		m_selfTransform = GetGameObject()->GetComponent<Transform>();

		CreateRectObject();
	}

	void RectDraw::OnUpdate() {
		if (auto rectObjectTransform = m_rectObjectTransform.lock()) {
			rectObjectTransform->SetPosition(m_selfTransform.lock()->GetPosition());
		}
	}

	void RectDraw::CreateRectObject() {
		//オブジェクト生成
		auto object = GetStage()->Instantiate<GameObject>(m_rect.centerPosition, Quat::Identity());
		
		//頂点とインデックスデータの生成
		auto buildParam = Builder::VertexPCTParametor(Vec3(1.0f) ,Vec2(0.0f, 0.0f) , L"");	//正方形用のセルを設定
		auto data = Builder::BuilderVertexPCT(buildParam);
		//表示コンポーネントのアタッチ
		auto draw = object->AddComponent<PNTStaticDraw>();
		draw->SetOriginalMeshUse(true);
		draw->CreateOriginalMesh(data.m_vertices, data.m_indices);

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