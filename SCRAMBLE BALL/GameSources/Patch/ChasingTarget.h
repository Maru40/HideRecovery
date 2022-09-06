#pragma once

namespace basecross
{
	class ChasingTarget : public Component
	{
		ex_weak_ptr<Transform> m_targetTransform;
	public:
		ChasingTarget(std::shared_ptr<GameObject>& owner) :
			Component(owner)
		{

		}

		void SetTarget(const std::shared_ptr<GameObject>& target)
		{
			m_targetTransform = target->GetComponent<Transform>();
		}

		void OnUpdate() override
		{
			if (!m_targetTransform)
			{
				return;
			}

			auto position = m_targetTransform->GetWorldPosition();
			transform->SetWorldPosition(position);
		}
	};
}