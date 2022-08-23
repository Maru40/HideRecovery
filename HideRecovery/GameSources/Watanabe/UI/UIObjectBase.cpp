#include "stdafx.h"
#include "UIObjectBase.h"

namespace basecross {
	UIObjectBase::UIObjectBase(const shared_ptr<Stage>& stage, const wstring& name)
		:GameObject(stage), m_name(name)
	{}

	void UIObjectBase::OnPreCreate() {
		GameObject::OnPreCreate();
		rectTransform = AddComponent<RectTransform>();
		rectTransform->SetPosition(m_rectTransformData.Position);
		rectTransform->SetScale(m_rectTransformData.Scale);
		rectTransform->SetRotation(m_rectTransformData.Rotation);

		AddTag(m_name);
	}
}