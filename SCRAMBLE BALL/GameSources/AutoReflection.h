/*!
@file AutoReflection.h
@brief AutoReflection
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	//--------------------------------------------------------------------------------------
	///	自動反射
	//--------------------------------------------------------------------------------------
	class AutoReflection : public Component
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		AutoReflection(const std::shared_ptr<GameObject>& objPtr);

		void OnCollisionEnter(const CollisionPair& pair) override;
	};

}