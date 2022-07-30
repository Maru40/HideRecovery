/*!
@file Crusher.h
@brief 破壊者判定
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//-----------------------------------------------------------------------------------------------
	/// スライムを潰せるオブジェクトかを判定するコンポーネントクラス
	//-----------------------------------------------------------------------------------------------
	class Crusher : public Component
	{
	public:
		//-----------------------------------------------------------------------------------------------
		/// コンストラクタ
		Crusher(const std::shared_ptr<GameObject>& ptrObj)
			:Component(ptrObj)
		{}
		//-----------------------------------------------------------------------------------------------
	};
}