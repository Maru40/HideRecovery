/*!
@file ItemBase.h
@brief ItemBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// アイテムの基底クラス
	//--------------------------------------------------------------------------------------
	class ItemBase : public Component
	{
		bool m_isAcquisition;		//獲得できる状態かどうか

	public:
		ItemBase(const std::shared_ptr<GameObject>& objPtr);

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 獲得できる状態かどうか
		/// </summary>
		/// <returns>獲得できる状態ならtrue</returns>
		bool IsAcquisition() const noexcept { return m_isAcquisition; };

		/// <summary>
		/// 獲得できる状態かどうかの設定
		/// </summary>
		/// <param name="isAcquisition">獲得できる状態かどうか</param>
		void SetIsAcquisition(const bool isAcquisition);
	};

}