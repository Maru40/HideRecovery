/*!
@file GraphAstar.h
@brief GraphAstarなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "SparseGraph.h"

namespace basecross {

	//-----------------------------------------------------------------------------------------------
	// 前方宣言
	//-----------------------------------------------------------------------------------------------
	class NavGraphNode;
	class AstarEdge;

	//-----------------------------------------------------------------------------------------------
	// Astar用のグラフ
	//-----------------------------------------------------------------------------------------------
	class AstarGraph : public SparseGraph<NavGraphNode, AstarEdge>
	{
		Vec3 m_centerPosition;	//グラフの中心位置

	public:
		AstarGraph(bool digraph);

		virtual ~AstarGraph() = default;

		/// <summary>
		/// 中心位置の計算
		/// </summary>
		/// <returns>中心位置</returns>
		Vec3 CalculateCenterPosition();

		//-----------------------------------------------------------------------------------------------
		// アクセッサ
		//-----------------------------------------------------------------------------------------------

		/// <summary>
		/// 中心位置の取得
		/// </summary>
		/// <returns>中心位置</returns>
		Vec3 GetCenterPosition() const noexcept { return m_centerPosition; }
	};

}