
/*!
@file AstarEdge.h
@brief AstarEdgeなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class GraphAstar;

	//--------------------------------------------------------------------------------------
	///	Astar用のエッジ
	//--------------------------------------------------------------------------------------
	class AstarEdge
	{
		//エッジは二つのノードを連結する。
		int m_from;      //手前のノードのインデックス
		int m_to;        //先のノードのインデックス

		float m_cost;    //移動のコスト

		bool m_isActive; //アクティブ状態

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="from">手前のインデックス</param>
		/// <param name="to">先のインデックス</param>
		AstarEdge(const int& from, const int& to);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="from">手前のインデックス</param>
		/// <param name="to">先のインデックス</param>
		/// <param name="cost">移動コスト</param>
		AstarEdge(const int& from, const int& to, const float& cost);

		virtual ~AstarEdge() = default;

		//-----------------------------------------------------------------------------------------------
		// アクセッサ
		//-----------------------------------------------------------------------------------------------

		/// <summary>
		/// 手前のインデックスをセット
		/// </summary>
		/// <param name="from">手前のインデックス</param>
		/// <returns>手前のインデックス</returns>
		void SetFrom(const int& from) noexcept { m_from = from; }

		/// <summary>
		/// 手前のインデックスを取得
		/// </summary>
		/// <returns>手前のインデックス</returns>
		int GetFrom() const noexcept { return m_from; }

		/// <summary>
		/// 先のインデックスのセット
		/// </summary>
		/// <param name="to">先のインデックス</param>
		void SetTo(const int& to) noexcept { m_to = to; }

		/// <summary>
		/// 先のインデックスの取得
		/// </summary>
		/// <returns>先のインデックス</returns>
		int GetTo() const noexcept { return m_to; }

		/// <summary>
		/// 移動コストのセット
		/// </summary>
		/// <param name="cost">移動コスト</param>
		void SetCost(const float& cost) noexcept { m_cost = cost; }

		/// <summary>
		/// 移動コストの取得
		/// </summary>
		/// <returns>移動コスト</returns>
		float GetCost() const noexcept { return m_cost; }

		/// <summary>
		/// アクティブ状態のセット
		/// </summary>
		/// <param name="isActive">アクティブ状態</param>
		void SetIsActive(const bool isActive) noexcept { m_isActive = isActive; }

		/// <summary>
		/// アクティブ状態の取得
		/// </summary>
		/// <returns>アクティブならtrue</returns>
		bool IsActive() const noexcept { return m_isActive; }

		/// <summary>
		/// from,to,両方のノードがActive状態かどうかを判断
		/// </summary>
		/// <param name="astar">自分が所属するGraphAstar</param>
		/// <returns>両方Activeならtrue</returns>
		bool IsNodeActive(const std::shared_ptr<GraphAstar>& astar) const;

	};

}

//endbasecross