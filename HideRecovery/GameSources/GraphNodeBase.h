
/*!
@file GraphNodeBase.h
@brief GraphNodeBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	グラフノードの規定クラス
	//--------------------------------------------------------------------------------------
	class GraphNodeBase
	{
		bool m_isActive; //アクティブな状態かどうか
		int m_index;     //ノードインデックス

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		GraphNodeBase();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="index">このクラスのインデックス</param>
		GraphNodeBase(const int& index);

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// インデックスの設定
		/// </summary>
		/// <param name="index">インデックス</param>
		void SetIndex(const int& index) noexcept { m_index = index; }

		/// <summary>
		/// インデックスの取得
		/// </summary>
		/// <returns>インデックス</returns>
		int GetIndex() const noexcept { return m_index; }

		/// <summary>
		/// アクティブ状態を設定
		/// </summary>
		/// <param name="isActive">アクティブ状態</param>
		void SetIsActive(const bool isActive) noexcept { m_isActive = isActive; }

		/// <summary>
		/// アクティブ状態を取得
		/// </summary>
		/// <returns>アクティブ状態かどうか</returns>
		bool IsActive() const noexcept { return m_isActive; }

	};

}