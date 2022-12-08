
/*!
@file RectDraw.cpp
@brief RectDrawのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "RectDraw.h"

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
		auto object = GetStage()->Instantiate<GameObject>(m_rect.centerPosition, Quat::Identity());
		auto draw = object->AddComponent<PNTStaticDraw>();
		draw->SetMeshResource(L"DEFAULT_CUBE");
		
		//draw->SetOriginalMeshUse(true);

		const auto& width = m_rect.width;
		const auto& depth = m_rect.depth;
		auto objTrans = object->GetComponent<Transform>();
		objTrans->SetScale(Vec3(width, 0.1f, depth));

		m_rectObject = object;
		m_rectObjectTransform = objTrans;
	}

}