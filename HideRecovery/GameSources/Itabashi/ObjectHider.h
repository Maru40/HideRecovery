#pragma once

namespace basecross
{
namespace Operator
{
	class ObjectHider : public Component
	{
		std::weak_ptr<Transform> m_transform;

	public:

		ObjectHider(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void Hide();
		void Hide(const Vec3& position);

		void Appear();
		void Appear(const Vec3& position);
	};
}
}