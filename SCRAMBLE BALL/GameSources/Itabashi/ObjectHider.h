#pragma once

namespace basecross
{
namespace Operator
{
	/// <summary>
	/// 隠れる機能を持ったコンポーネント
	/// </summary>
	class ObjectHider : public Component
	{
		/// <summary>
		/// トランスフォーム
		/// </summary>
		std::weak_ptr<Transform> m_transform;

	public:

		ObjectHider(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		/// <summary>
		/// 隠れる(非表示にする)
		/// </summary>
		void Hide();
		/// <summary>
		/// 位置を指定して隠れる(非表示にする)
		/// </summary>
		/// <param name="position">隠れる位置</param>
		void Hide(const Vec3& position);

		/// <summary>
		/// 現れる(表示する)
		/// </summary>
		void Appear();
		/// <summary>
		/// 位置を指定して現れる(表示する)
		/// </summary>
		/// <param name="position">現れる位置</param>
		void Appear(const Vec3& position);
	};
}
}