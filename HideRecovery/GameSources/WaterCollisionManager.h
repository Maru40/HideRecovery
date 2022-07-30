/*!
@file WaterCollisionManager.h
@brief 水の衝突判定
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//------------------------------------------------------------------------------------------------
	/// ジェットポンプの衝突判定コンポーネントクラス
	//------------------------------------------------------------------------------------------------
	class WaterCollisionManager : public Component
	{
	public:
		//-----------------------------------------------------------------------------------------------
		/// コンストラクタ
		WaterCollisionManager(const std::shared_ptr<GameObject>& ptrObj)
			:Component(ptrObj)
		{}
		//-----------------------------------------------------------------------------------------------
		void OnCollisionEnter(const CollisionPair& pair) override;
		void OnCollisionExcute(const CollisionPair& pair) override;
		//-----------------------------------------------------------------------------------------------
	};
}