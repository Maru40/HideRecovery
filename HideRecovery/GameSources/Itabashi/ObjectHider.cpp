#include "stdafx.h"
#include "ObjectHider.h"

namespace basecross
{
namespace Operator
{
	ObjectHider::ObjectHider(const std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{

	}

	void ObjectHider::OnCreate()
	{
		m_transform = GetGameObject()->GetComponent<Transform>();
	}

	void ObjectHider::Hide()
	{
		GetGameObject()->SetActive(false);
	}

	void ObjectHider::Hide(const Vec3& position)
	{
		auto transform = m_transform.lock();

		if (transform)
		{
			transform->SetWorldPosition(position);
		}

		Hide();
	}

	void ObjectHider::Appear()
	{
		GetGameObject()->SetActive(true);
	}

	void ObjectHider::Appear(const Vec3& position)
	{
		auto transform = m_transform.lock();

		if (transform)
		{
			transform->SetWorldPosition(position);
		}

		Appear();
	}
}
}