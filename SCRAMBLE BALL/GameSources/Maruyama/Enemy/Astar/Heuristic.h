/*!
@file Heuristic.h
@brief Heuristic
担当者：丸山 裕喜
*/

#pragma once

#include "NavGraphNode.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	目的のノードを記憶して、そのノードとの計算をするクラス。
	//--------------------------------------------------------------------------------------
	class Heuristic
	{
		ex_weak_ptr<NavGraphNode> m_targetNode;  //目的地に設定したノード

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		Heuristic();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="node">目的地に設定したノード</param>
		Heuristic(const std::shared_ptr<NavGraphNode>& node);

		virtual ~Heuristic() = default;

		/// <summary>
		/// 渡されたノードからターゲットノードまでの距離を測って返す。
		/// </summary>
		/// <param name="node">開始地点のノード</param>
		/// <returns>targetNode.position - node.position</returns>
		float CalculateHeuristicRange(const std::shared_ptr<const NavGraphNode>& node);

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 目的地に設定したいノードをセット
		/// </summary>
		/// <param name="node">目的地に設定したいノード</param>
		void SetTargetNode(const std::shared_ptr<NavGraphNode>& node) noexcept { m_targetNode = node; }
	};

}

//endbasecross