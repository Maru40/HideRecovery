#include "stdafx.h"
#include "UIObjectBase.h"

namespace basecross {
	UIObjectBase::UIObjectBase(const shared_ptr<Stage>& stage)
		:GameObject(stage)
	{}

	void UIObjectBase::OnPreCreate() {
		GameObject::OnPreCreate();
		rectTransform = AddComponent<RectTransform>();
		auto rectTrans = rectTransform.lock();
		rectTrans->SetPosition(m_rectTransformData.Position);
		rectTrans->SetScale(m_rectTransformData.Scale);
		rectTrans->SetRotation(m_rectTransformData.Rotation);

		//AddTag(L"UI");
	}
}