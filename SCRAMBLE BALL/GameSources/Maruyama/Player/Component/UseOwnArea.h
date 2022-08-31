
/*!
@file UseOwnArea.h
@brief UseOwnAreaなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 自陣エリアを使用するクラス
	//--------------------------------------------------------------------------------------
	class UseOwnArea : public Component
	{
	public:
		UseOwnArea(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;
	};

}