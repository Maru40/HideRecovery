/*!
@file GraphEdgeBase.h
@brief GraphEdgeBase
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	エッジの基底クラス
	//--------------------------------------------------------------------------------------
	class GraphEdgeBase
	{
		int m_from;   //手前のノードインデックス
		int m_to;     //先のノードインデックス

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="from">手前のノードインデックス</param>
		/// <param name="to">先のノードインデックス</param>
		GraphEdgeBase(const int& from, const int& to)
			:m_from(from), m_to(to)
		{}

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 手前のノードインデックスの取得
		/// </summary>
		/// <returns>手前のノードインデックス</returns>
		int GetFrom() const noexcept { return m_from; }

		/// <summary>
		/// 先のノードインデックスの取得
		/// </summary>
		/// <returns>先のノードインデックス</returns>
		int GetTo() const noexcept { return m_to; }
	};

}