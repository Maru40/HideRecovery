#include "stdafx.h"
#include "UIObjectBase.h"

namespace basecross {
	UIObjectBase::UIObjectBase(const shared_ptr<Stage>& stage, const wstring& name)
		:GameObject(stage), m_uiName(name)
	{}

	void UIObjectBase::OnPreCreate() {
		GameObject::OnPreCreate();
		m_rectTransform = AddComponent<RectTransform>();
		m_rectTransform->SetPosition(m_rectTransformData.Position);
		m_rectTransform->SetScale(m_rectTransformData.Scale);
		m_rectTransform->SetRotation(m_rectTransformData.Rotation);

		AddTag(m_uiName);
	}
}