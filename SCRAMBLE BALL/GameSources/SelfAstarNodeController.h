
/*!
@file SelfAstarNodeController.h
@brief SelfAstarNodeControllerなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class NavGraphNode;

	//--------------------------------------------------------------------------------------
	///	自分が現在どのノードに所属するかを記録する。
	//--------------------------------------------------------------------------------------
	class SelfAstarNodeController : public Component
	{
		bool m_isNodeInitialize;

		std::weak_ptr<NavGraphNode> m_node;

	public:
		SelfAstarNodeController(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnLateStart() override;
		void OnUpdate() override;

	private:
		void UpdateProccess();

		/// <summary>
		/// ノードの更新
		/// </summary>
		void UpdateNode();

		/// <summary>
		/// ノードの更新が必要かどうか
		/// </summary>
		/// <returns></returns>
		bool IsUpdateNode();

		/// <summary>
		/// ノードの初期化
		/// </summary>
		void InitializeNode();

		/// <summary>
		/// 所持ノードが遠すぎるかどうか
		/// </summary>
		/// <returns>遠すぎるならtrue</returns>
		bool IsFarNode();

	public:
		std::shared_ptr<NavGraphNode> GetNode() const noexcept;

		void SetNode(const std::shared_ptr<NavGraphNode>& node) noexcept;

		bool HasNode() const noexcept;

		_NODISCARD bool IsInitializeNode() const noexcept { return m_isNodeInitialize; };

	};

}