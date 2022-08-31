/*!
@file I_BrackBoard.h
@brief I_BrackBoardなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ブラックボードのインターフェース
	//--------------------------------------------------------------------------------------
	template<class Struct>
	class I_BrackBoard
	{
		Struct m_blackBoardParam;  //ブラックボード

	public:
		/// <summary>
		/// ブラックボードのパラメータの設定
		/// </summary>
		/// <param name="param">ブラックボードのパラメータ</param>
		virtual void SetBlackBoardParam(const Struct& param) {
			m_blackBoardParam = param;
		}

		/// <summary>
		/// ブラックボードのパラメータの取得
		/// </summary>
		/// <returns>ブラックボードのパラメータ</returns>
		virtual Struct GetBlackBoardParam() const {
			return m_blackBoardParam;
		}

		/// <summary>
		/// ブラックボードのパラメータの参照を取得
		/// </summary>
		/// <returns>ブラックボードのパラメータの参照</returns>
		virtual Struct& GetRefBlackBoardParam() {
			return m_blackBoardParam;
		}
	};

}

//basecross