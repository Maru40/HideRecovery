
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
		bool m_isNodeInitialize;	//将来的にはなくしたい。

		bool m_isMover;				//動く者なら更新をかける。

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

		//初めのエリアになかった場合に再起してノードを検索する。
		std::shared_ptr<NavGraphNode> SearchNode(const int firstAreaIndex, const int currentAreaIndex, const int direction = -1);

		/// <summary>
		/// 所持ノードが遠すぎるかどうか
		/// </summary>
		/// <returns>遠すぎるならtrue</returns>
		bool IsFarNode();

	public:
		/// <summary>
		/// 手動で現在ノードを更新したい時に利用する。
		/// </summary>
		/// <returns>現在のノード</returns>
		std::shared_ptr<NavGraphNode> CalculateNode();

		void SetNode(const std::shared_ptr<NavGraphNode>& node) noexcept;

		std::shared_ptr<NavGraphNode> GetNode() const noexcept;

		bool HasNode() const noexcept;

		_NODISCARD bool IsInitializeNode() const noexcept { return m_isNodeInitialize; };

		void SetIsMover(const bool isMover) noexcept { m_isMover = isMover; }

		_NODISCARD bool IsMover() const noexcept { return m_isMover; }

	};

}