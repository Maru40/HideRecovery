
/*!
@file UseWepon.h
@brief UseWeponなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	template<class BulletType>
	class WeponBase;

	//--------------------------------------------------------------------------------------
	/// ウェポンを使用するクラス
	//--------------------------------------------------------------------------------------
	class UseWepon : public Component
	{

	public:
		UseWepon(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

		
	};

}